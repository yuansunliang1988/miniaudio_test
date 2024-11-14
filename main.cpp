#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"
#include <iostream>
#include <fstream>

#define OUTPUT_FILE "output.raw"

// �ص�����������˷��ȡ��Ƶ���ݲ����浽�ļ�
void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount) {
    (void)pDevice; // ����δʹ�þ���
    (void)pOutput; // ������ֻ������������
    std::ofstream* pFile = static_cast<std::ofstream*>(pDevice->pUserData);
    if (pFile && pInput) {
        pFile->write(reinterpret_cast<const char*>(pInput), frameCount * ma_get_bytes_per_frame(pDevice->capture.format, pDevice->capture.channels));
    }
}

int main() {
    ma_result result;
    ma_device_config config;
    ma_device device;
    std::ofstream outFile;

    // ������ļ�
    outFile.open(OUTPUT_FILE, std::ios::binary);
    if (!outFile.is_open()) {
        std::cerr << "Failed to open file for writing.\n";
        return -1;
    }

    // ������Ƶ�豸
    config = ma_device_config_init(ma_device_type_capture); // ����Ϊ¼���豸
    config.capture.format = ma_format_s16;                 // 16λ����
    config.capture.channels = 2;                           // ˫��������������
    config.sampleRate = 48000;                             // ������ 44100Hz
    config.dataCallback = data_callback;                   // �������ݻص�
    config.pUserData = &outFile;                           // ���ļ�ָ�봫�ݸ��ص�

    // ��ʼ���豸
    result = ma_device_init(NULL, &config, &device);
    if (result != MA_SUCCESS) {
        std::cerr << "Failed to initialize capture device.\n";
        outFile.close();
        return -1;
    }

    // �����豸
    result = ma_device_start(&device);
    if (result != MA_SUCCESS) {
        std::cerr << "Failed to start capture device.\n";
        ma_device_uninit(&device);
        outFile.close();
        return -1;
    }

    std::cout << "Recording... Press Enter to stop.\n";
    std::cin.get();

    // ֹͣ�豸������
    ma_device_uninit(&device);
    outFile.close();

    std::cout << "Recording finished. Saved to " << OUTPUT_FILE << "\n";
    return 0;
}
