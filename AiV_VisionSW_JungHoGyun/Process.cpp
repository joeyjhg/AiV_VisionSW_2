#pragma once
#include "Common.h"
#include "Process.h"
#include "ImageObject.h"
#include "Logging.h"
#include "Bulr.h"
#include "CustomBlur.h"

Process::Process()
{
    if (!initialize())
        return;
    thread_Start();
}

Process::~Process()
{
    for (auto& s : src)
        delete s;
}



//i��°�� ���� �о����
bool Process::initialize()
{
    kernel = 21;
    num_threads = 3;
    resultimg_path = readFile("setting.txt", 2);
    mtx = make_unique<mutex>(); // �����ڿ��� mutex ��ü ����
    if (resultimg_path == "")
        return false;
    src.resize(num_threads);  // src ���� ũ�� �ʱ�ȭ
    return true;
}

// ������ ����
void Process::thread_Start()
{
    vector<thread> threads;			// ������ ����
    for (int i = 0; i < num_threads; i++)
        threads.push_back(thread(&Process::Start, this, i));    // i+1 ��° ������ ����

    for (auto& thread : threads)
        if (thread.joinable())
            thread.join();   // ������ ����
}

// New_processImage ����
void Process::Start(int i)
{
    {
        lock_guard<mutex> lock(*mtx);    // ������ ����ȭ
        writeFile("image_blur.log", to_string(i + 1) + " Thread Start");    // log
        cout << i + 1 << "��° ������ ����" << endl;
    }

    src[i] = new imageobject_src(); // vector<imageobject_src*> src �ʱ�ȭ
    cout << i + 1 << "��° ���� �̹��� ���� �Ϸ�" << endl;

    // New_processImage ����
    if (!New_processImage(src[i])) 
    {
        lock_guard<mutex> lock(*mtx);
        cout << i + 1 << "��° ������ ������ ����" << endl;
        delete src[i];
        return;
    }
    delete src[i];
    {
        lock_guard<mutex> lock(*mtx);
        writeFile("image_blur.log", to_string(i + 1) + " Thread Complete");
        cout << i + 1 << "��° ������ ����" << endl;
    }
}

// �̹��� �¾�
bool Process::New_processImage(imageobject_src* src)
{
    // dst1,2 ����
    imageobject_dst dst1;
    imageobject_dst dst2;

    // OpenCV_bulr�� Custom_blur dst�� ����
    bool successOpenCV = OpenCV::return_ImageBlur_OpenCV(src, &dst1, kernel);
    bool successCustom = Custom::return_ImageBlur_Custom(src, &dst2, kernel);
    if (!successOpenCV || !successCustom) {
        return false;
    }
    if (!CompareImage(dst1, dst2))
        return false;

    return true;
}

// �̹��� ��
bool Process::CompareImage(const imageobject_dst& dst1, const imageobject_dst& dst2)
{
    cout << "�� ����" << endl;
    // �� �ȼ����� ���� unsigned char*�� ���� ������ �Ҵ�
    const unsigned char* img_OpenCV = dst1.getBuffer();
    const unsigned char* img_Custom = dst2.getBuffer();

    int width = dst1.getWidth();
    int height = dst1.getHeight();

    // �̶� unsigned char*���� 1���� �迭�̹Ƿ� [n][0]�� [n-1][0]���� height��ŭ ����������.
    for (int i = 0; i < width; ++i)
    {
        for (int j = 0; j < height; ++j)
        {
            int idx = i * width + j;    // idx���� i*height + j�� �����Ͽ� ���ʷ� ��
            if (img_OpenCV[idx] != img_Custom[idx]) 
            {
                lock_guard<mutex> lock(*mtx);
                writeFile("image_blur.log", "The gray values of the image pixels are different.");    // log
                cout << "�̹��� �ٸ�" <<i<<" "<<j<< endl;
                return false;
            }
        }
    }

    {
        lock_guard<mutex> lock(*mtx);
        writeFile("image_blur.log", "The gray values of the image pixels are identical.");    // log
        cout << "�̹��� ����" << endl;
    }
    // �̹��� ����
    SaveImage(dst1, dst2);
    delete img_OpenCV;
    delete img_Custom;
    return true;
}

// �̹��� ����
void Process::SaveImage(const imageobject_dst& dst1, const imageobject_dst& dst2)
{
    // imageobject���� Mat�� ȣȯ�Ǵ� �Ű������� ����
    Mat blurredImg_OpenCV(dst1.getHeight(), dst1.getWidth(), CV_8UC1, const_cast<uchar*>(dst1.getBuffer()));
    Mat blurredImg_Custom(dst2.getHeight(), dst2.getWidth(), CV_8UC1, const_cast<uchar*>(dst2.getBuffer()));

    string time_str = returnTime();
    bool saved = false;
    int i = 1;
    // ������ ���� �̸� ����
    while (!saved) 
    {
        string resultimg_path_OpenCV = "." + resultimg_path + "/blurred_OpenCV_" + time_str + "_" + to_string(i) + ".png";
        string resultimg_path_Custom = "." + resultimg_path + "/blurred_Custom_" + time_str + "_" + to_string(i) + ".png";

        {
            // ���� ���� ���� Ȯ��
            if (!ifstream(resultimg_path_OpenCV) && !ifstream(resultimg_path_Custom)) 
            {
                // ���� ���� �� ��� ���
                lock_guard<mutex> lock(*mtx);
                if (!ifstream(resultimg_path_OpenCV) && !ifstream(resultimg_path_Custom)) 
                {
                    if (imwrite(resultimg_path_OpenCV, blurredImg_OpenCV) && imwrite(resultimg_path_Custom, blurredImg_Custom)) 
                    {
                        cout << "Save OK" << endl;
                        writeFile("image_blur.log", "Save Image_" + resultimg_path_OpenCV); // log
                        writeFile("image_blur.log", "Save Image_" + resultimg_path_Custom); // log
                        saved = true;
                    }
                    else
                        cout << "Save Fail" << endl;
                }
            }
        }

        if (!saved)
            i++;
        
    }
}