# qnze-gamepad
Arduino Nano based gamepod<br>

qnze-gamepod Arduino nano gamepod &amp; software<br>
できるだけハンダつけ作業をつけしなくてもいいようにブレッドボードを使っています。<br>
テトリスを遊びやすくするためにボタンを５つ使います。<br>
たまたま音が２つ出せたのでスピーカーを２つ使っています。<br>
電池ボックスもオプションで追加できます。<br>

<b>必要な部品</b><br>
Arduino Nano x1<br>
OLED SSD1306 0.96インチ x1<br>
ブレッドボードミニ x2<br>
ミニブレッドボード 25穴タイプ x1<br>
タクトスイッチ6x6x8 x5<br>
パッシブブザー12x8.5mm ピッチ6.5mm x2 (ブザーとなっていますがスピーカーです)<br>
ブレッドボード用ジャンパー線　多数<br>
(もしバッテリーパックが必要なら) 電池ボックス 単4電池×4 直列 ON/OFFスイッチ付き<br>
ケース　3Dプリンタ用のデータがstlフォルダにあります。<br>
<br>
<img src=./images/aq-gamepod1_ブレッドボード.jpg height=600><br>
(作図ソフトにミニブレッドボード 25穴タイプの部品図がなかったのでブレッドボードミニで代用しています)<br>
<br>
接続 <br>
D8 Speaker1<br>
D9 Speaker2<br>
D2 Shoot button<br>
A0 Left button<br>
A1 Up button<br>
A2 Down button<br>
A3 Right button<br>
A4 OLED SDA<br>
A5 OLED SCA<br>
5V OLED VCC<br>
GND OLED GND<br>
<br>
２つの音が同時に出ています。<br>
Sound demo https://youtu.be/YCTtuJd_oEE<br>
Sound demo BACH BWV578 https://youtu.be/WstahqAys5A<br>
テトリス https://youtu.be/a2FQSlDUryo<br>
インベーダー https://youtu.be/MdoJukn5BSs<br>

<br>
<img src=./images/a.jpg width=600><br>
<img src=./images/c.jpg width=600><br>

<img src=./images/photo1.jpg width=600><br>
十字キーのほうが操作性がいいです<br>
<img src=./images/photo2.jpg width=600><br>
カバーと電池は本体にひっかけるタイプ<br>
<img src=./images/photo3.jpg width=600><br>
3層構造になっていて、電池パックは最下部にくっつけます<br>
