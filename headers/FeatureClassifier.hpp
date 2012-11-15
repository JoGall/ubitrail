#ifndef FEATURECLASSIFIER_H
#define FEATURECLASSIFIER_H
#include "opencv2/imgproc/imgproc.hpp"
#include "defines.hpp"

//todel
#include <iostream>

/**
 * The class responsible for solving ambiguities if several valid contours are detected.
 * It relies on a custom implementations of likelihood model.
 * When exactly one valid contous is present, FeatureClassifier learns.
 * Otherwise, the FeatureClassifier decided which contour is valid.
 * The class also keeps informations about the last valid contours that can be queried by public methods
 */
class FeatureClassifier
{
    public:
        /**
         * The default constructor.
         */
        FeatureClassifier();

        /**
         * Another contructor. A FeatureClassifier object cannot be used unless it was constructed in this manner.
         * @param roundToTrain the number of learning occurences to be preformed before the model should be used.
         */
        FeatureClassifier(int roundToTrain);
        virtual ~FeatureClassifier();

        /**
         * A public member function updating the current classifier.
         * @param contours an output vector of contours.
         * @param img an input frame it is used to calculates come features.
         *
         * It performs as follow:
         * 1. Calculates features for each contour using makeFeatures();
         * 2. Calculates the likelihood of each contours based on the features and the feature history using calcLikelihood().
         * 2. If precisely one contours is present, the model is updated via updateMeanSD().
         * 3. Else, the contours with the highest likelyhood is kept

         */
        void run(std::vector<std::vector<cv::Point> >& contours, cv::Mat& img);

        /**
         * A public member function assessing whether or not FeatureClassifier is trained.
         * @return true if the model if the FeatureClassifier is trained.
         */
        bool isTrained(){
                return !(roundTrained < m_roundToTrain);
        }

        /**
         * A public member function getting the training progress for FeatureClassifier.
         * @return the proportion ot training.
         */
        float trainedFract(){
            return (float) roundTrained/ (float)m_roundToTrain;
        }

        /**
         * A public member function getting the likelihood of the last accepted contour.
         * @return the last best log(likelihood).
         */
        float getL(){
            return L;
        }

        /**
         * A public member function getting the position of the last accepted contour.
         * @return the last accepted position.
         */
        cv::Point2f getPosition(){
//            return realPosition;
            return position;
        }
        //TODEL
        void printFeatures(){
            std::cout<<"TAG"<<L<<","<<position<<","<<realPosition<<","<<featuresMeansSds<<std::endl;
        }
    protected:
        void makeMiniImg(std::vector<cv::Point>& contour,   cv::Mat& miniIMG,  cv::Mat& img);

        /**
         * A protected member function making features.
         * @param featureVec a reference to a vector of double containing the list of features.
         * @param countour the contour to calculate features on.
         * @param miniIMG the ROI in the original image corresponding to the bounding box of the contour.
         *
         * The current implemented features are:
         * - Surface of the contour.
         * - Decimal logarithm of the distance of the contour center to the last accepted contour center.
         * - Mean of pixel values in the blue channel.
         * - Mean of pixel values in the green channel.
         * - Mean of pixel values in the red channel.
         * - Standard deviation of pixel values in the blue channel.
         * - Standard deviation of pixel values in the green channel.
         * - Standard deviation of pixel values in the red channel.
         */
        void makeFeatures(std::vector<double>& featureVec,std::vector<cv::Point>& contour,  cv::Mat& miniIMG);

        /**
         * A protected member function calculating the log-likelihood of a given vector of features.
         * @param vect a vector of features, for instance, obtained via makeFeatures().
         * @return the log-likelihood L.
         *
         * L is calculated under the assumption of normal distribution of the features: \n
         *  \f$ L = \sum_{i=0}^{n} l_i \f$ \n
         * Where: \n
         *  \f$ l_i = \ln{(\frac{1}{s_i\sqrt{2\pi}} e^{-\frac{(x_i - m_i)^2}{2s_i^2 }})} \f$ \n
         * With: \n
         * \f$ i \f$ is a given feature. \n
         * \f$ x_i \f$ is the value of the feature \f$ i\f$ (i.e. vect[i]). \n
         * \f$ m_i \f$ is a pseudo-mean for the feature \f$ i\f$. \n
         * \f$ s_i \f$ is a pseudo-standard-deviation for the feature \f$ i\f$. \n
         * \f$ m_i \f$ and \f$ s_i \f$ are defined by updateMeanSD(). \n
         */
        float calcLikelihood(std::vector<double>& vect);

        /**
         * A protected member function updating the pseudo-mean and standard deviations of the features.
         * @param vect a vector of features, for instance, obtained via makeFeatures().
         *
         * The function performs a weighted runing average (cv::accumulateWeighted) on the vector of features to update the pseudo-mean. \n
         * To update the pseudo-sd, the function calculates the absolute differences (deviations) between each feature and there respective pseudo-means
         * and perfoms another weighted runing average on these deviations.
         * The defauld weight is FEATURE_CLASSIF_ACCU_WEIGHT (defines);
         */
        void updateMeanSD(std::vector<double>& vect);

        cv::Point2f updatePosition (cv::Point2f center);
    private:
        cv::Mat featuresMeansSds;
        cv::Mat positionsAccum;
        unsigned long roundTrained;
        unsigned int m_roundToTrain;
        cv::Point2f position,realPosition;
        float L;
};

#endif // FEATURECLASSIFIER_H
