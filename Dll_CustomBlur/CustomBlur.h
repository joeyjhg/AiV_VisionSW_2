#pragma once
class ImageObject;
namespace Custom {
	extern "C"
	{
		// Bulr ó�� Custom �Լ�
		__declspec(dllexport) bool ImageBlur(const ImageObject* src, ImageObject* dst, const int kernelSize);
		// Bulr ó�� �Լ� ����
		__declspec(dllexport) bool return_ImageBlur_Custom(const ImageObject* src, ImageObject* dst, const int kernelSize);
	}
}

