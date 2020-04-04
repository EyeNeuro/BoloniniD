import imageio as imio
import sys


if len (sys.argv) > 1:
    s = sys.argv[1]
    try:
        img_array = imio.imread(s)
    except Exception:
        print("File", s, "not found!")
        exit
    f = open('neuro_input.txt', 'w')
    for i in img_array:
        for j in i:
            f.write(str(j)+" ")
    f.close()
else:
    print("Filename not found.")
