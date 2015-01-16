#python ParrotGenerator.py -i ./neuralnet/$1.essence.json -o ./parrot/$1.$2.parrot.cpp -t CPU -c ./config/$2.json
python ParrotGenerator.py -i ./neuralnet/$1.essence.json -o ./parrot/$1.$2.parrot.cpp -t AVX -c ./config/$2.json
