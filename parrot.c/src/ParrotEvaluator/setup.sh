mkdir -p Bench/$1/$2
mkdir -p Bench/$1/$2/out
touch Bench/$1/$2/out/hadianeh.txt
ln -s ../../../Template/Makefile.$1 Bench/$1/$2/Makefile
ln -s ../../../Template/ParrotEvaluator.cpp Bench/$1/$2/

ln -s ../../../../KeyToFann/$2.data Bench/$1/$2/parrot.data
ln -s ../../../../KeyToFann/$2.nn Bench/$1/$2/parrot.nn

ln -s ../../../../ParrotGenerator/parrot/$2.$1.parrot.cpp Bench/$1/$2/

ls -lha -R Bench/$1/$2

#cat Bench/$1/$2/*.*

