mkdir -p bin/res/

rm bin/res/*
cp -a res/* bin/res/

gcc ./src/main.c -o ./bin/redgreen -O2 -Wall -Wno-unused-result -Wno-missing-braces -I ./include/ -L ./lib/ -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 
