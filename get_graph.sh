#/bin/bash
frq=$1
funcName="sin_$frq"
wavName="$funcName.wav"

./sinus_generator $1
./fft $wavName

generate_graph() {
    gnuplot <<- _EOF_
        set terminal postscript eps enhanced color solid
        set output "graph_$funcName.ps"

        set grid xtics ytics
        #set xtics  25
        set log yx
        set xlabel "Freq, Hz"
        set ylabel "Amp, dB"

        set xrange [1:22050]
        #set yrange [0.00001:100000]

        plot "test.txt" using 1:2 title "AFC" with lines linestyle 1
_EOF_
    return
}

generate_graph

#clear
rm *.txt *.wav
