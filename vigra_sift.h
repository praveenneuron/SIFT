//
// Created by someone on 25.04.16.
//

#ifndef SIFT_HELPER_VIGRA_H
#define SIFT_HELPER_VIGRA_H

#include <vigra/impex.hxx>
#include <vigra/multi_fft.hxx>
#include <vigra/multi_array.hxx>
#include <vigra/convolution.hxx>
#include <vigra/gaborfilter.hxx>
#include <vigra/edgedetection.hxx>
#include <vigra/hdf5impex.hxx>
#include <vigra/imageinfo.hxx>

#define PI 3.1415926535897932384626433832795

namespace vigra
{

    /**
     * A Class that serves as structure for storing the keypoint related
     * information.
     */
    class KeyPoint
    {
    public:
        float ptx;
        float pty;
        float size;
        float angle;
        float response;
        int octave;
        int r;
        int c;
        int intvl;
        float scale_octv;

    };

    /**
     * Class for extracting descriptors for provided keypoint.
     */
    class VigraSiftDescriptor
    {
        int features;
        int octaveLayers;
        double contrastThreshold;
        double edgeThreshold;
        double sigma;
        MultiArray<2, vigra::UInt8> image_array;
        MultiArray<2, vigra::UInt8> descriptor_array;
        std::vector<KeyPoint> key_points;
        std::vector<MultiArray<2, vigra::UInt8>> gaussian_pyramid;


        // Width of descriptor histogram array
        static const int DESCR_WIDTH = 4;

        // Number of bins per histogram in descriptor array
        static const int DESCR_HIST_BINS = 8;

        // determines the size of a single descriptor orientation histogram
        static constexpr float DESCR_SCL_FCTR = 3.f;

        // threshold on magnitude of elements of descriptor vector
        static constexpr float DESCR_MAG_THR = 0.2f;

        // factor used to convert floating-point descriptor to unsigned char
        static constexpr float INT_DESCR_FCTR = 512.f;

        public:

        /**
         * Method to set the values to be used by VigraSiftDescriptor
         */
        void setValues(
                int features,
                int octaveLayers,
                double contrastThreshold,
                double edgeThreshold,
                double sigma
        );

        /**
         * Get the size of descriptor.
         */
        int getDescriptorSize();

        /**
         * Allocate and initialize image array.
         */
        void allocateAndInitializeImage(const char*  file_name);

        /**
         * Allocate and initialize descriptor array.
         */
        void allocateDescriptorArray();

        /**
         * Get and set the keypoints extracted from earlier stage.
         */
        void setKeypoints(std::vector<KeyPoint> key_points);

        /**
         * Build gaussian pyramid.
         */
        void build_gauss_pyr();

        /**
         * Helper subroutine to calculate descriptors for given keypoint.
         */
        float* calculate_descriptors_helper(
                const MultiArray<2, vigra::UInt8> img,
                float ptx,
                float pty,
                float orientation,
                float size);

        /**
         * Main method responsible for getting the correct image in which you
         * want to extract features. Makes the call to helper routine which
         * basically extracts the descriptors.
         */
        float* calculate_descriptors(int keypoint_id);


    };

    /**
     * Class for extracting key points for provided image and building Gaussian
     * Pyramid.
     */
    class VigraSiftDetector {
        MultiArray<2, UInt8> src_img;
        int octaves;
        int intervals;
        double sigma;
        double contr_thr;
        double curv_thr;
        std::vector<MultiArray<2, UInt8> > gauss_pyr;
        std::vector<MultiArray<2, UInt8> > dog_pyr;
        std::vector<vigra::KeyPoint> keypoints;
        MultiArray<1, double> hist;

        // default number of sampled intervals per octave
        static const int SIFT_INTVLS = 3;

        // default sigma for initial gaussian smoothing
        static constexpr float SIFT_SIGMA = 1.6;

        /** default threshold on keypoint contrast |D(x)| */
        static constexpr float SIFT_CONTR_THR = 0.04;

        /** default threshold on keypoint ratio of principle curvatures */
        static constexpr int SIFT_CURV_THR = 10;

        /* assumed gaussian blur for input image */
        static constexpr float SIFT_INIT_SIGMA = 0.5;

        /* width of border in which to ignore keypoints */
        static const int SIFT_IMG_BORDER = 5;

        /* maximum steps of keypoint interpolation before failure */
        static const int SIFT_MAX_INTERP_STEPS = 5;

        /* default number of bins in histogram for orientation assignment */
        static const int SIFT_ORI_HIST_BINS = 36;

        /* determines gaussian sigma for orientation assignment */
        static constexpr float SIFT_ORI_SIG_FCTR = 1.5;

        /* determines the radius of the region used in orientation assignment */
        static constexpr double SIFT_ORI_RADIUS = 3.0 * SIFT_ORI_SIG_FCTR;

        /* number of passes of orientation histogram smoothing */
        static const int SIFT_ORI_SMOOTH_PASSES = 2;

        /* orientation magnitude relative to max that results in new feature */
        static constexpr float SIFT_ORI_PEAK_RATIO = 0.8;

        static constexpr float SIFT_FIXPT_SCALE = 48.0;

    };

}



#endif //SIFT_HELPER_VIGRA_H
