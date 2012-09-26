#include "FeatureClassifier.hpp"
//TODO benchmarck row/col

FeatureClassifier::FeatureClassifier(){

}


FeatureClassifier::FeatureClassifier(int roundToTrain):
roundTrained(0),
featuresMeansSds(N_FEATURES,2,CV_64F,cv::Scalar(50)),
m_roundToTrain(roundToTrain)
{
    //ctor
}

FeatureClassifier::~FeatureClassifier()
{
    //dtor
}

void FeatureClassifier::run(std::vector<std::vector<cv::Point> >& contours,cv::Mat& img){

    if(contours.size() > 0){
        cv::Mat miniImg;
        std::vector<std::vector< double> > featureArrays(contours.size());
        std::vector< double> likelyhoods(contours.size());

        // We calculate features of all objects
        for(unsigned int i = 0; i < contours.size(); i++){
            featureArrays[i].resize(N_FEATURES);
            makeMiniImg(contours[i],miniImg,img);
            makeFeatures(featureArrays[i],contours[i],miniImg);
            if(roundTrained > 1){
                likelyhoods[i] = this->calcLikelyhood(featureArrays[i]);
            }
        }

        unsigned int bestIdx=0;
        float bestLikelihood = -std::numeric_limits<float>::max();

        if(contours.size() == 1 ){
            this->updateMeanSD(featureArrays[0]);
            bestLikelihood = likelyhoods[0];
            if(roundTrained < m_roundToTrain){
                roundTrained++;
            }
        }

        else{
            for(unsigned int i = 0; i < contours.size(); i++){
                if(likelyhoods[i] > bestLikelihood){
                    bestIdx = i;
                    bestLikelihood = likelyhoods[bestIdx];
                }
            }
        }
    L=bestLikelihood;
    contours[0] = contours[bestIdx];
    contours.resize(1);
    }

}


void FeatureClassifier::makeMiniImg(std::vector<cv::Point>& contour,  cv::Mat& miniIMG, cv::Mat& img ){
    cv::Rect ROI;
    ROI = cv::boundingRect(contour);
    miniIMG = img(ROI);
}

void FeatureClassifier::makeFeatures(std::vector<double>& featureVec,std::vector<cv::Point>& contour,  cv::Mat& miniIMG){

    featureVec[FEATURE_AREA_IDX] = contourArea(contour);

    cv::Moments moms = cv::moments(contour);

    cv::RotatedRect Rect = cv::minAreaRect(contour);
    cv::Point2f rRect[4];

        double y,x;

    Rect.points(rRect);
    if(moms.m00 >0){
        x = moms.m10/moms.m00;
        y = moms.m01/moms.m00;
    }
    else{
        x = (rRect[0].x+rRect[2].x) /2;
        y = (rRect[0].y+rRect[2].y) /2;
    }

    cv::Point2f center(x,y);

    double dist, dx,dy;
    dx = (center.x - position.x);
    dy = (center.y - position.y);
    dist = sqrt(dx*dx + dy*dy);
    featureVec[FEATURE_DIST_FROM_FORMER_IDX] = log10(dist+1e-6);

    std::vector<std::vector<cv::Point> > temporaryContours(1);
    temporaryContours[0] = contour;

    cv::Mat mask(miniIMG.rows,miniIMG.cols,CV_8U,cv::Scalar(0));


    std::vector<cv::Point2f> pts(4);
    pts[0] = rRect[0];
    pts[1] = rRect[1];
    pts[2] = rRect[2];
    pts[3] = rRect[3];

    cv::Rect  bRect = cv::boundingRect(pts);
    cv::drawContours(mask,temporaryContours,0,cv::Scalar(255),-1,8,cv::noArray(),INT_MAX,cv::Point(-bRect.x,-bRect.y));
    cv::Scalar m,s;
    cv::meanStdDev(miniIMG,m,s);
    featureVec[FEATURE_BMEAN_IDX] = m[0];
    featureVec[FEATURE_GMEAN_IDX] = m[1];
    featureVec[FEATURE_RMEAN_IDX] = m[2];

    featureVec[FEATURE_BSD_IDX] = s[0];
    featureVec[FEATURE_GSD_IDX] = s[1];
    featureVec[FEATURE_RSD_IDX] = s[2];

    realPosition = center;
    position = this->updatePosition(center);
}

cv::Point2f FeatureClassifier::updatePosition (cv::Point2f center){
    float data[2];
    data[0] = center.x;
    data[1] = center.y;
    assert(!(center.x != center.x) && !(center.y != center.y) );

    cv::Mat positAsMat(1,2,CV_32F,data);
    if(positionsAccum.empty())
        positAsMat.copyTo(positionsAccum);

    cv::accumulateWeighted(positAsMat,positionsAccum,0.3); //MAGIC NUMBER
    return cv::Point2f(positionsAccum.at<float>(0,0) ,positionsAccum.at<float>(0,1));

}

float FeatureClassifier::calcLikelyhood(std::vector<double>& vect){


    cv::Mat tmp(vect);
    cv::Mat deviations(N_FEATURES,1,CV_64F,cv::Scalar(0));
    cv::Mat means = featuresMeansSds.col(0);
    cv::Mat sds = featuresMeansSds.col(1);
    cv::Mat sqrSDs;
    cv::Mat A;
    cv::Mat B;
    cv::Mat liks;

    /* The deviation d */
    cv::absdiff(means, tmp,deviations);
    cv::pow(deviations,2,deviations);
    cv::pow(sds,2,sqrSDs);
    cv::divide(deviations,sqrSDs,A,0.5);
    cv::exp(-A,A);
    B = SQRT2PI*sds;
    liks = A/B;
    cv::log(liks,liks);
    float logL = sum(liks)[0];
    return logL;
}

void FeatureClassifier::updateMeanSD(std::vector<double>& vect){
    cv::Mat tmp(vect);
    cv::Mat means = featuresMeansSds.col(0);
    cv::Mat sds = featuresMeansSds.col(1);
    cv::Mat absDevs;

    if(roundTrained > 0){
        cv::absdiff(means, tmp,absDevs);
        cv::accumulateWeighted(absDevs,sds,FEATURE_CLASSIF_ACCU_WEIGHT);
    }

    cv::accumulateWeighted(tmp,means,FEATURE_CLASSIF_ACCU_WEIGHT);
}

