clear
cat << "INFO"
========================================================
WebBrowser基础信息:

程序版本:v20.10beta
菜单版本:v2.1
作者:https://t.me/YamaiSakuragi

========================================================
[1]安装依赖   [2]配置程序   [3]删除整体   [*]退出菜单
========================================================
*输入对应数字启动服务
INFO
ER(){
pkg update -y &&
pkg install x11-repo gtk3 webkit2gtk-4.1 pulseaudio gstreamer gst-plugins-base gst-plugins-good gst-libav clang make cmake pkg-config -y
read -p "*回车返回"
exit 0
}
EH(){
clang++ $(dirname "$0")/assets/WebBrowser.cpp -o $(dirname "$0")/assets/WebBrowser $(pkg-config --cflags --libs gtk+-3.0 webkit2gtk-4.1) -std=c++17 -Wno-deprecated-declarations
mkdir -p $PREFIX/share/WebBrowser
mkdir -p $HOME/.config/WebBrowser/data/icons/
mkdir -p $PREFIX/share/icons/hicolor/64x64/apps/
mkdir -p $PREFIX/share/applications/
mkdir -p $HOME/Desktop/
cp $(dirname "$0")/res/WebBrowser.png $PREFIX/share/icons/hicolor/64x64/apps/
cp $(dirname "$0")/assets/Frontend_WebBrowser.desktop $HOME/Desktop/
cp $(dirname "$0")/assets/Rearend_WebBrowser.desktop $PREFIX/share/applications/
cp $(dirname "$0")/assets/WebBrowser $PREFIX/bin/
chmod +x $HOME/Desktop/Frontend_WebBrowser.desktop
chmod +x $PREFIX/share/applications/Rearend_WebBrowser.desktop
chmod +x $PREFIX/bin/WebBrowser
read -p "*回车返回"
exit 0
}
EM(){
rm -rf $PREFIX/share/WebBrowser
rm -rf $PREFIX/share/icons/hicolor/64x64/apps/WebBrowser.png
rm -rf $HOME/Desktop/Frontend_WebBrowser.desktop
rm -rf $PREFIX/share/applications/Rearend_WebBrowser.desktop
rm -rf $PREFIX/bin/WebBrowser
rm -rf $HOME/.config/WebBrowser/
apt autoremove
pkg clean
read -p "*回车返回"
exit 0
}
cd $(dirname "$0")
while true; do
  read -s -n1 key
    case $key in
       1) clear && ER ;;
       2) clear && EH ;;
       3) clear && EM ;;
       *) exit 0 ;;
    esac
  done