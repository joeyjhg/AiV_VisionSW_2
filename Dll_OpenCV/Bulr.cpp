#include "pch.h"
#include "Bulr.h"
#include "ImageObject.h"
#include "Logging.h"

#define EXPORTDLL extern "C" __declspec(dllexport)

namespace OpenCV {
    // Bulr ó�� OpenCV �Լ�
    bool ImageBlur(const ImageObject* src, ImageObject* dst, const int kernelSize)
    {
        writeFile("image_blur.log", "OpenCV bulr() Start");
        // Ŀ���� ¦���ų� 1���� �۴ٸ�
        if (kernelSize % 2 == 0 || kernelSize < 1)
            return false;
        // �̹��� ������ �޾ƿ���
        Mat imgMat(src->getHeight(), src->getWidth(), CV_8UC1, src->getBuffer());

        if (imgMat.empty())
            return false;

        Mat blurred;
        // OpenCV blur�Լ� �̿�
        blur(imgMat, blurred, Size(kernelSize, kernelSize));

        // dst ��ü�� ������ ����
        vector<uchar> blurredData(blurred.datastart, blurred.dataend);
        dynamic_cast<imageobject_dst*>(dst)->set_image_object(blurredData, blurred.cols, blurred.rows);

        writeFile("image_blur.log", "OpenCV bulr() Complete");

        return true;
    }
    bool return_ImageBlur_OpenCV(const ImageObject* src, ImageObject* dst, const int kernelSize)
    {
        return OpenCV::ImageBlur(src, dst, kernelSize);
    }
}

