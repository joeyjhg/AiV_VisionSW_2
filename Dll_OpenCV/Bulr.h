#pragma once
class ImageObject;
namespace OpenCV {
	extern "C"
	{
		// Bulr ó�� OpenCV �Լ�
		__declspec(dllexport) bool ImageBlur(const ImageObject* src, ImageObject* dst, const int kernelSize);
		// Bulr ó�� �Լ� ����
		__declspec(dllexport) bool return_ImageBlur_OpenCV(const ImageObject* src, ImageObject* dst, const int kernelSize);
	}
}


