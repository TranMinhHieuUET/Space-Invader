* Họ và tên: Trần Minh Hiếu
* Mã sinh viên: 24020132
* Chủ đề game: Space Invader - Vào vai một tàu chiến vũ trụ tiêu diệt người ngoài hành tinh đến xâm lược.
* Ngưỡng điểm cho là xứng đáng: 9
* Lí do:
+ Game có các tính năng:
- Cơ chế tăng độ khó: Mỗi wave mới đều có tốc độ nhanh hơn wave trước, mỗi lần chạm biên 2 bên màn hình đi xuống đều được tăng tốc độ, cứ mỗi 2 wave sẽ tăng số lượng kẻ địch bắn đạn về người chơi cùng 1 lúc (max 15 kẻ địch). 
- Tăng sức mạnh của người chơi: Có các powerup có tỉ lệ xuất hiện khi kẻ địch bị tiêu diệt (hiện tại là 10%), có 4 power up: tăng tốc độ, tăng tốc độ bắn, thêm mạng và invincibility (1 khoảng thời gian không thể bị trúng đạn của kẻ địch).
- Iframes cho người chơi: Mỗi khi người chơi trúng đạn sẽ được reset vị trí về vị trí gốc và có 1 khoảng thời gian không thể bị trúng đạn (để tránh trường hợp trúng đạn liên tiếp khiến người chơi thua lập tức).
- 2 chế độ chơi: Có 2 chế độ chơi riêng biệt: 1 người chơi (tính điểm), 2 người chơi (đối kháng).
- Lưu lại high score: Sử dụng file txt để lưu trữ lại điểm của người chơi, chỉ giữ lại 10 điểm cao nhất.
- Có các menu riêng biệt: Sử dụng enum Gamestate để quản lí trạng thái của game, từ đó tạo ra các menu riêng biệt (Main menu, choose mode, high score, pause, game over, guide).
- Có sound effects và âm nhạc: sử dụng SDL_Mixer.
- Render điểm và mạng của người chơi trong lúc đang chơi bằng SDL_Ttf.
- Quản lí đạn trong game bằng các vector.
- Chuyển động mượt: Sử dụng biến Delta time để update chuyển động của game theo giây (không theo từng frame) giúp cho các chuyển động trong game mượt và giống nhau trên các thiết bị có frame rate khác nhau.
- Hiệu ứng của các nút: Khi di chuột lên sẽ có hiệu ứng hình ảnh, khi bấm sẽ có sound effect.
+ Ban đầu có tham khảo code AI, nhưng về sau khi thêm các tính năng khác đã có sửa đổi so với code AI, tự thêm nhiều tính năng.
+ Game có đồ hoạ không quá đơn giản.
+ Có các thuật toán tự viết (cơ chế tăng độ khó của game bằng cách tăng tốc độ của kẻ địch, tăng số kẻ địch bắn đạn trong 1 lần, các powerup để tăng sức mạnh của người chơi)
+ Tự học và sử dụng các thư viện SDL_Mixer, SDL_Img, SDL_Ttf.
* Mức độ sử dụng AI: 40%.
* Các nguồn tham khảo:
- Sound effect: https://pixabay.com/sound-effects/search/8-bit/
- Âm nhạc: https://www.youtube.com/watch?v=mRN_T6JkH-c&list=PLwJjxqYuirCLkq42mGw4XKGQlpZSfxsYd
- AI: https://aistudio.google.com/app/prompts?state=%7B%22ids%22:%5B%221bSR5cVYkdykb0xd84Q0eN_WwObxYOl8p%22%5D,%22action%22:%22open%22,%22userId%22:%22102793632749442712213%22,%22resourceKeys%22:%7B%7D%7D&usp=sharing
- Lazyfool (Học SDL): https://lazyfoo.net/tutorials/SDL/index.php
- Ảnh background: https://www.google.com/search?sca_esv=8d80958cb2551b43&sxsrf=AHTn8zrI5dpA_jC2de5ZAuW-0HW--FXAGg:1743225617091&q=8+bit+space+background&udm=2&fbs=ABzOT_AfCikcO6SgGMxZXxAG9tmS8rx53CbgOCSVg3O9Xo5xAK_RXi3VFy8QcDJV9F46BNXKXwox_WSCQV76ijiqBdBg6NRqVlHKQc3pwBtQoGz9SnRtkaemTDujBQ_WcyL1PAzyagoV5brOoVXOrpbUkx-ZCa1bebw030Tt2-vC3V_NSR4TP5ilqWhWCsFxSX_P_w0u8dUlgEi_BgjsZ-8dmBfRm3Xm_URb0P4iCrpl5xheVQnF1M7tkCIqU_690ZKPU_RbI-pR&sa=X&ved=2ahUKEwiV5q29xa6MAxXpfvUHHXp3H14QtKgLegQIGBAB&biw=1912&bih=920&dpr=1
- Alien spritesheet: https://www.kindpng.com/imgv/iixRwwo_space-invaders-sprites-png-transparent-png/
