#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"
#include <iostream>
#include <fstream>

#define OUTPUT_FILE "output.raw"

// 回调函数：从麦克风获取音频数据并保存到文件
void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount) {
    (void)pDevice; // 避免未使用警告
    (void)pOutput; // 本例中只处理输入数据
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

    // 打开输出文件
    outFile.open(OUTPUT_FILE, std::ios::binary);
    if (!outFile.is_open()) {
        std::cerr << "Failed to open file for writing.\n";
        return -1;
    }

    // 配置音频设备
    config = ma_device_config_init(ma_device_type_capture); // 配置为录音设备
    config.capture.format = ma_format_s16;                 // 16位整型
    config.capture.channels = 2;                           // 双声道（立体声）
    config.sampleRate = 48000;                             // 采样率 44100Hz
    config.dataCallback = data_callback;                   // 设置数据回调
    config.pUserData = &outFile;                           // 将文件指针传递给回调

    // 初始化设备
    result = ma_device_init(NULL, &config, &device);
    if (result != MA_SUCCESS) {
        std::cerr << "Failed to initialize capture device.\n";
        outFile.close();
        return -1;
    }

    // 启动设备
    result = ma_device_start(&device);
    if (result != MA_SUCCESS) {
        std::cerr << "Failed to start capture device.\n";
        ma_device_uninit(&device);
        outFile.close();
        return -1;
    }

    std::cout << "Recording... Press Enter to stop.\n";
    std::cin.get();

    // 停止设备并清理
    ma_device_uninit(&device);
    outFile.close();

    std::cout << "Recording finished. Saved to " << OUTPUT_FILE << "\n";
    return 0;
}
