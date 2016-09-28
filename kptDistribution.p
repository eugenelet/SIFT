set title "Keypoints Distribution (64 pixels)"
set xlabel"Keypoints Number"
set ylabel "Keypoints Count"
plot  "kptDist.dat" using 1:2   with linespoints
