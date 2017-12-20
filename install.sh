cd judgelight/src/
gcc -fPIC -shared judgelight.c limit.c listen.c run.c -o judgelight.so -I/usr/include/python2.7/ -lpython2.7
cd ../
mkdir ~/.judgelight
cp __init__.py ~/.judgelight/judgelight.py
cp src/judgelight.so ~/.judgelight/judgelight.so
cd ~/.judgelight
sudo chmod 777 ./judgelight.py
sudo rm /usr/local/bin/judgelight
sudo ln -s ~/.judgelight/judgelight.py /usr/local/bin/judgelight
echo "安装成功，您可以通过 judgelight [命令] 的方式来使用"