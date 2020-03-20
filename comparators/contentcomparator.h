#ifndef CONTENTCOMPARATOR_H
#define CONTENTCOMPARATOR_H

#include <QVector>




#include <opencv2/opencv.hpp>

#include "imagecomparator.h"
#include "../fastpairqueue.h"



class ContentComparator : public ImageComparator
{
    Q_OBJECT


public:
    enum Type {
        Content = 201
    };
    ContentComparator();
    ContentComparator(QVector<cv::Mat> *imagesMat, QVector<FileInfo> *baseFiles,
                      Queue<QString> *in, Queue<QString> *out = 0,
                      int minWidth = 0, int minHeight = 0);

    void setImagesMat(QVector<cv::Mat> *imagesMat);
    void deleteImagesMat();


private:
    bool compareImages(const cv::Mat &image1, const cv::Mat &image2);

signals:

public slots:

private:
    void run();

    QVector<cv::Mat> *imagesMat;
    int minWidth;
    int minHeight;

};

#endif // CONTENTCOMPARATOR_H
