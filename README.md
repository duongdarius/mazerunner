# mazerunner
Review Code & Test Game: https://drive.google.com/file/d/1S8dLPxsPvm-MQxz-LRIybmXRNxuPIggj/view?usp=drive_link

## I. GIỚI THIỆU GAME:

Maze Runner là một trò chơi 2D phiêu lưu trong mê cung, được phát triển bằng SDL2. Người chơi sẽ điều khiển nhân vật vượt qua các thử thách, tiêu diệt quái vật và tìm tới kho báu để giành chiến thắng.

## II. CHỨC NĂNG CHÍNH:

- Di chuyển nhân vật trong mê cung bằng các phím mũi tên.

- Bắn đạn tiêu diệt quái vật với health bar hiển thị.

- Thu thập vật phẩm để tăng tốc độ di chuyển và tăng số lượng đạn bắn.

- 2 màn chơi với độ khó tăng dần:  
  - Level 1: Bọ cạp xuất hiện (Scorpio enemy)
  - Level 2: Xác ướp xuất hiện (Mummy enemy)

- Animation di chuyển mượt mà cho quái vật và nhân vật.
- Chế độ Pause (P) và màn hình thắng thua.

## III. HƯỠNG DẪN CHƠI GAME
## 1. Bắt đầu game
- Khi khởi động trò chơi, người chơi sẽ được đưa tới màn hình **START NEW  GAME**.
- Tại đây, người chơi có thể lựa chọn màn chơi trước khi bắt đầu cuộc phiêu lưu.
<img width="707" alt="{74649E34-8DA9-4D0D-9956-1E65ABEC3932}" src="https://github.com/user-attachments/assets/31501d0b-aa90-4bd0-8d4c-4a6575069aa6" />

## 2. Chọn level
- Level 1: Đối đầu với bọ cạp (Scorpio) trong mê cung cơ bản.
- Level 2: Đối đầu với xác ướp (Mummy) trong mê cung khó hơn.

<img width="713" alt="{F062B358-C844-46E0-AE12-BECFC1D1E156}" src="https://github.com/user-attachments/assets/9679fa78-71a0-41fc-88dc-58845cf77b0d" />

## 3. Các thành phần trong game
- **Người chơi (Player)**: Nhân vật chính, có khả năng di chuyển và bắn đạn.

  <img width="46" alt="{7FBCBFDA-81AA-4728-A14E-3465D3DEB3EB}" src="https://github.com/user-attachments/assets/9d4f942d-4812-4c40-98cb-40b54ce1e2ff" />

- **Kẻ địch (Enemy)**:
  - Level 1: Bọ cạp (Scorpio)
  
  <img width="47" alt="{77D5368F-A64E-496F-8CAA-5B58E586D0F3}" src="https://github.com/user-attachments/assets/38e2a4f9-11ef-4bef-911c-e643e54a816a" />

  - Level 2: Xác ướp (Mummy)
    
    <img width="31" alt="{ED28DBDF-A919-4863-BCBA-11617A04AA4E}" src="https://github.com/user-attachments/assets/933fb94c-7cf8-4e5d-914a-af239b49960e" />

- **Vật phẩm (Items)**:
  - Tăng tốc độ di chuyển.
    
  <img width="44" alt="{ABC7C593-F911-4FB6-A3D0-73C39441DF80}" src="https://github.com/user-attachments/assets/fba2f6b2-8be5-470b-86e3-3c65ba479cef" />
  
  - Tăng số lượng đạn bắn ra.
    
  <img width="35" alt="{047DA8E1-202A-41C6-9C05-8E8D01ED4284}" src="https://github.com/user-attachments/assets/39a93be5-1f1d-4d26-8dfb-861f70ce3165" />

- **Bụi cỏ (Bush)**:
  - Bụi cỏ là vật cản đặc biệt trong mê cung.
  - Khi đạn của người chơi bắn trúng bụi cỏ, đạn sẽ **biến mất**.
    
  <img width="61" alt="{039E9BAA-609B-463A-B924-F14E9E7CE0DF}" src="https://github.com/user-attachments/assets/5fbab9c1-c5ed-49a4-a6cc-a739cebc8f29" />

- **Kho báu (Treasure)**:
  - Điểm kết thúc của mỗi mê cung.
    
  <img width="52" alt="{D68086F6-D134-4DE9-ACBD-49A45FB599AF}" src="https://github.com/user-attachments/assets/4237d916-e607-4e40-b58a-f7e02dd57e22" />
  
  - Người chơi cần tìm tới kho báu để chiến thắng.

## 4. Cách chơi
- **Di chuyển**: Dùng các phím mũi tên trên bàn phím để di chuyển nhân vật.
- **Bắn đạn**: Dùng phím Space để bắn đạn tiêu diệt quái vật.
- **Pause game**: Bấm phím P để tạm dừng hoặc tiếp tục trò chơi.
- **Chọn level**: Dùng phím lên xuống + Enter tại màn hình Start.

## 5. Chiến thắng và thất bại
- **Chiến thắng**: Người chơi tìm tới kho báu thành công mà không bị kẻ địch tiêu diệt.

- **Thất bại**: Người chơi bị kẻ địch va chạm và tiêu diệt.
Khi chiến thắng ➔ Hiển thị màn hình `YOU WIN!`

<img width="329" alt="{840C54F5-26EF-433A-9CAA-565E7AE04D0D}" src="https://github.com/user-attachments/assets/4bfe014d-e69e-4526-8c00-c2f4c2e08d47" />

Khi thất bại ➔ Hiển thị hộp thoại `YOU LOSE`.

<img width="309" alt="{AEA7748A-6AD3-4EFA-900A-44CD87B8B1C9}" src="https://github.com/user-attachments/assets/c24b9147-e724-495e-9830-c314d003d783" />

## IV. Về đồ họa của game
## 📂 assets
- `ammo.png`: Icon vật phẩm đạn (free asset từ itch.io).
- `background.png`: Background dùng ở màn hình start và pause (miễn phí từ Freepik).
- `bullet.png`: Hình viên đạn (sprite tự tạo / nguồn mở).
- `enemy.png`: Sprite enemy tĩnh – Scorpio (free pixel art từ itch.io).
- `explosion.png`: Spritesheet hiệu ứng nổ (free sprite explosion từ opengameart.org).
- `floor.png`: Nền mặt sàn mê cung (free pixel asset từ Freepik).
- `goal.png`: Icon kho báu (treasure) (free asset từ itch.io).
- `Mummy_walk.png`: Spritesheet xác ướp di chuyển (Mummy) (miễn phí từ itch.io).
- `Scorpio_walk.png`: Spritesheet bọ cạp di chuyển (Scorpio) (miễn phí từ itch.io).
- `wall.png`: Bụi cỏ / vật cản trong mê cung (free tile từ itch.io).
- `speed.png`: Icon vật phẩm tăng tốc độ (free asset từ itch.io).
- `you_win.png`: Bảng thông báo chiến thắng (free pixel UI từ opengameart.org).
- `player_run_0.png → player_run_7.png`: 8 frame hoạt ảnh nhân vật chạy (free animated sprite từ itch.io).
- `shoot.wav`: Âm thanh bắn đạn (nguồn miễn phí từ freesound.org).
- `hit.wav`: Âm thanh trúng đạn (nguồn miễn phí từ freesound.org).
- `win.wav`: Âm thanh chiến thắng (nguồn miễn phí từ freesound.org).
- `font.ttf`: Font chữ sử dụng cho text (Google Fonts - miễn phí bản quyền).

## V. Về source code game
- **Ngôn ngữ lập trình**: C++
- **Thư viện**:
  - SDL2
  - SDL2_image
  - SDL2_mixer
  - SDL2_ttf
- **IDE đề xuất**: Code::Blocks
- **Cấu trúc project**:
  - **draw** (chức năng vẽ - không tách riêng, tích hợp vào `main.cpp`):
  - Khởi tạo window và renderer bằng SDL2.
  - Load và vẽ các texture (ảnh, chữ) lên màn hình game.
  - Gọi SDL2, SDL2_image, SDL2_ttf để xử lý hình ảnh, font chữ.

- **elements/**
  - **player.cpp / player.h**:
    - Quản lý hành động nhân vật (di chuyển, bắn đạn).
    - Xử lý va chạm với vật phẩm, kẻ địch.
  - **enemy.cpp / enemy.h**:
    - Quản lý kẻ địch (Enemy) - bao gồm di chuyển, đổi hướng khi chạm tường.
    - Hiển thị animation kẻ địch.
    - Xử lý máu và các tương tác khi bị trúng đạn.

- **map/**
  - **maze.cpp / maze.h**:
    - Khởi tạo bản đồ mê cung cho game.
    - Sinh ra mê cung dạng lưới với tường, bụi cỏ, kho báu.
    - Sinh vị trí spawn quái vật.

- **events/**
  - **main.cpp**:
    - Xử lý vòng lặp game (game loop).
    - Xử lý sự kiện bàn phím:
      - Di chuyển nhân vật.
      - Bắn đạn.
      - Pause game (P).
      - Chọn level (Start Screen).

- **logic/**
  - (Tích hợp trong `main.cpp`):
    - Update trạng thái nhân vật, enemy, đạn theo thời gian thực.
    - Xử lý ăn vật phẩm, va chạm đạn với tường, bụi cỏ hoặc enemy.
    - Kiểm tra điều kiện thắng/thua.

- **music/**
  - Tích hợp SDL2_mixer để phát âm thanh hiệu ứng (shoot, hit).

## VI. NHỮNG NGUỒN SỬ DỤNG ĐỂ TẠO GAME
Công cụ AI hỗ trợ khác như: ChatGPT.

Chuyển âm thanh sang dạng wav.

Xoá phông nền hình ảnh.
