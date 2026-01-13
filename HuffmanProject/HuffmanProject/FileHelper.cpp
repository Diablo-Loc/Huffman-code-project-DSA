#include "FileHelper.h"
#include <fstream>

std::vector<BYTE> FileHelper::readBinary(const std::string& path) {
    // 1. Mở file và di chuyển con trỏ xuống cuối file ngay lập tức
    std::ifstream file(path, std::ios::binary | std::ios::ate);

    if (!file) return std::vector<BYTE>();

    // 2. Lấy kích thước file (vị trí hiện tại của con trỏ)
    std::streamsize size = file.tellg();
    if (size <= 0) return std::vector<BYTE>();

    // 3. Quay lại đầu file
    file.seekg(0, std::ios::beg);

    // 4. Cấp phát bộ nhớ 1 lần duy nhất cho toàn bộ dữ liệu
    std::vector<BYTE> data(size);

    // 5. Đọc toàn bộ khối dữ liệu vào vector
    if (file.read((char*)data.data(), size)) {
        return data;
    }

    return std::vector<BYTE>();
}

void FileHelper::writeBinary(const std::string& path,
    const std::vector<BYTE>& data) {
    std::ofstream file(path, std::ios::binary);
    if (!file) return;

    file.write((char*)data.data(), data.size());
}
