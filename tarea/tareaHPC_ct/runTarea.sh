runTarea.sh

./codigo/tareaHPC 1024 50 ./results/
./codigo/tareaHPC 2048 50 ./results/
./codigo/tareaHPC $((1024*4)) 50 ./results/
./codigo/tareaHPC $((1024*8)) 50 ./results/
./codigo/tareaHPC $((1024*16)) 50 ./results/
./codigo/tareaHPC $((1024*32)) 50 ./results/

gnuplot ./Tarea.gnu






