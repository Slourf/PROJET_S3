# include <stdlib.h>
# include <stdio.h>
# include "main.h"

struct matrix* resize_char(struct matrix *mat, int size) {
    double xscale = (size+0.0) / mat->w;
    double yscale = (size+0.0) / mat->h;
    double threshold = 0.5 / (xscale * yscale);
    double yend = 0.0;

    struct matrix *new_mat = build_matrix(size, size);

    for (int f = 0; f < size; ++f) {
        double ystart = yend;
        yend = (f + 1) / yscale;
        if (yend >= mat->h)
            yend = mat->h - 0.000001;
        double xend = 0.0;
        for (int g = 0; g < size; ++g) {
            double xstart = xend;
            xend = (g + 1) / xscale;
            if (xend >= mat->w)
                xend = mat->w - 0.000001;
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
                    sum += mat->data[x][y] * yportion * xportion;
                }
            }
            new_mat->data[g][f] = (long)((sum > threshold) ? 1 : 0);
        }
    }
    return new_mat;
}
