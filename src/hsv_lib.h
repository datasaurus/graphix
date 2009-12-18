#ifndef HSV_H_
#define HSV_H_

void RGBtoHSV(double r, double g, double b, double *h, double *s, double *v);
void HSVtoRGB(double *r, double *g, double *b, double h, double s, double v);

#endif
