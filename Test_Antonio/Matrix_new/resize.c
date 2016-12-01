# include <stdlib.h>
# include <stdio.h>
# include "main.h"


long** resize_char(long **mat, int width, int height, int size) {
    double xscale = (size+0.0) / width;
    double yscale = (size+0.0) / height;
    double threshold = 0.5 / (xscale * yscale);
    double yend = 0.0;

    long** new_mat = build_matrix(size, size);

    for (int f = 0; f < size; ++f) {
        double ystart = yend;
        yend = (f + 1) / yscale;
        if (yend >= height)
            yend = height - 0.000001;
        double xend = 0.0;
        for (int g = 0; g < size; ++g) {
            double xstart = xend;
            xend = (g + 1) / xscale;
            if (xend >= width)
                xend = width - 0.000001;
            double sum = 0.0;
            for (int y = (int)ystart; y <= (int)yend; ++y) {
                double yportion = 1.0;
                if (y == (int)ystart)
                    yportion -= ystart - y;
                if (y == (int)yend)
                    yportion -= y + 1 - yend;
                for (int x = (int)xstart; x <= (int)xend; ++x) {
                    double xportion = 1.0;
                    if (x == (int)xstart)
                        xportion -= xstart - x;
                    if (x == (int)xend)
                        xportion -= x + 1- xend;
                    sum += mat[x][y] * yportion * xportion;
                }
            }
            new_mat[g][f] = (long)((sum > threshold) ? 1 : 0);
        }
    }
    return new_mat;
}
