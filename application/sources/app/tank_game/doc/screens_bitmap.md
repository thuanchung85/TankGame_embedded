=================  ENGLISH  ===============================

TECHNICAL DOCUMENTATION: GRAPHICS ENCODING MODULE (BITMAP)
    - Component file name: screens_bitmap.cpp
    - Accompanying header file: screens_bitmap.h
    - Target platform: Embedded System / Microcontroller using graphics library.

1. Module Overview
    The screens_bitmap.cpp module acts as the static graphics resource repository for the Tank game. Due to the limited RAM of microcontrollers or embedded chips, all image data (Sprite/UI) in this file is structured as monochrome unsigned char arrays and is configured to be stored directly in Program Memory (Flash Memory) instead of being loaded into RAM during operation.

2. Core Technical Components
    - Optimization Keyword: PROGMEM
    - Nature: A macro in the embedded environment (as specified by AVR/ARM).
    - Function: Instructs the compiler to place these graphics data arrays into Flash memory.
    - Benefits: Completely frees up static RAM space, preventing stack/heap overflow errors during game execution.

3. Detailed List of Graphics Resources (Sprites)
    Below is a table summarizing all graphics structures defined in the file, categorized by function:

    3.1. Entities & Combat Group
    https://docs.google.com/spreadsheets/d/1Xdx3CrvaS-TIHpDV2TaS8o8wnALDcpp_XXp8NSwU8XU/edit?usp=sharing

    3.2. Environment & Terrain Group
    https://docs.google.com/spreadsheets/d/1PZqZlBvlm6CQIRs_BhsRaNd_eGhfoBVnfhTjazFtbSU/edit?usp=sharing

    3.3. Effects & UI Group
    https://docs.google.com/spreadsheets/d/1aXfQ1feZMBZbtj9c5Hdj3pv5XJVRfqTLUHLJaeAE4Jc/edit?usp=sharing

4. Decoding and Display Principles (Hex to Pixel Encoding)
    All the data arrays above follow the principle of encoding monochrome images specifically for embedded displays (OLED/LCD):

    - Data format: Each bit 1 represents a pixel that is turned on (light color/drawing color), and bit 0 represents a pixel that is turned off (background color).

    - Scanning mechanism: Data is scanned in rows from left to right, from top to bottom.

    - How to calculate array size: Size (Bytes) = 8Width × Height (For example, with a bitmap_tank of size 30×30: Each 30px row needs 30/8 = 4 Bytes. The total of 30 rows will be 4×30 = 120 Bytes, which is completely accurate with the actual code).

5. Instructions for Using & Calling Functions in Source Code
    To draw these resources on the screen, programmers need to use bitmap drawing functions from Flash memory of compatible graphics libraries.

    Example function call syntax: display.drawBitmap(player_X, player_Y, bitmap_tank, 30, 30, WHITE);


=================   VIETNAMESE  ===========================

TÀI LIỆU KỸ THUẬT: MÔ-ĐUN LƯU TRỮ ĐỒ HỌA MÃ HÓA (BITMAP)

- Tên file thành phần: screens_bitmap.cpp
- File tiêu đề đi kèm: screens_bitmap.h
- Nền tảng mục tiêu: Hệ thống nhúng (Embedded System) / Vi điều khiển sử dụng thư viện đồ họa.

1. Tổng Quan Mô-đun
    Mô-đun screens_bitmap.cpp đóng vai trò là Bộ lưu trữ tài nguyên đồ họa (Asset Repository) tĩnh của trò chơi Tank. Do bộ nhớ RAM của các vi điều khiển hoặc chip nhúng thường rất hạn chế, toàn bộ dữ liệu hình ảnh (Sprite/UI) trong file này được cấu trúc dưới dạng các mảng byte (unsigned char) đơn sắc và được ép cấu hình lưu trữ trực tiếp trên Bộ nhớ chương trình (Flash Memory - PROGMEM) thay vì nạp vào RAM khi vận hành.

2. Các Thành Phần Kỹ Thuật Cốt Lõi
    - Từ khóa tối ưu hóa: PROGMEM
    - Bản chất: Là một macro trong môi trường nhúng (như AVR/ARM chỉ định).
    - Chức năng: Chỉ định cho trình biên dịch (Compiler) đặt các mảng dữ liệu đồ họa này vào vùng nhớ Flash.
    - Lợi ích: Giải phóng hoàn toàn dung lượng RAM tĩnh, ngăn ngừa lỗi tràn bộ nhớ (Stack/Heap Overflow) trong quá trình game chạy.

3. Danh Sách Chi Tiết Các Tài Nguyên Đồ Họa (Sprites)
    Dưới đây là bảng thống kê toàn bộ các cấu trúc đồ họa được định nghĩa trong file, phân loại theo nhóm chức năng:

    3.1. Nhóm Nhân Vật & Cơ Chế Chiến Đấu (Entities & Combat)
    https://docs.google.com/spreadsheets/d/1Xdx3CrvaS-TIHpDV2TaS8o8wnALDcpp_XXp8NSwU8XU/edit?usp=sharing
    
    3.2. Nhóm Môi Trường & Bản Đồ (Environment & Terrain)
    https://docs.google.com/spreadsheets/d/1PZqZlBvlm6CQIRs_BhsRaNd_eGhfoBVnfhTjazFtbSU/edit?usp=sharing

    3.3. Nhóm Hiệu Ứng & Giao Diện (Effects & UI)
    https://docs.google.com/spreadsheets/d/1aXfQ1feZMBZbtj9c5Hdj3pv5XJVRfqTLUHLJaeAE4Jc/edit?usp=sharing

4. Nguyên Lý Giải Mã và Hiển Thị (Mã hóa Hex sang Pixel)
    Toàn bộ các mảng dữ liệu trên đều tuân theo nguyên lý mã hóa hình ảnh đơn sắc chuyên dụng cho màn hình nhúng (OLED/LCD):
        - Định dạng dữ liệu: Mỗi bit 1 đại diện cho 1 pixel được bật (màu sáng/màu vẽ), và bit 0 đại diện cho pixel tắt (màu nền).
        - Cơ chế quét: Dữ liệu được quét theo dòng từ trái sang phải, từ trên xuống dưới.
        - Cách tính dung lượng mảng:Dung lượng (Bytes)=⌈8Width​⌉×Height(Ví dụ với bitmap_tank kích thước 30×30: Mỗi dòng 30 px cần ⌈30/8⌉=4 Bytes. Tổng 30 dòng sẽ là 4×30=120 Bytes hoàn toàn chính xác với code thực tế).

5. Hướng Dẫn Sử Dụng & Gọi Hàm Trong Source Code
    Để vẽ các tài nguyên này lên màn hình, lập trình viên cần sử dụng các hàm vẽ bitmap từ bộ nhớ Flash của các thư viện đồ họa tương thích.

    Cú pháp gọi hàm mẫu: display.drawBitmap(player_X, player_Y, bitmap_tank, 30, 30, WHITE);
