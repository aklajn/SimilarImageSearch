#include "contentcomparator.h"

ContentComparator::ContentComparator() :
    ImageComparator()
{
    imagesMat = 0;
    minWidth = 10;
    minHeight = 10;
}

ContentComparator::ContentComparator(QVector<cv::Mat> *imagesMat, QVector<FileInfo> *baseFiles,
                                     Queue<QString> *in, Queue<QString> *out,
                                     int minWidth, int minHeight) :
    ImageComparator(baseFiles, in, out)
{
    this->imagesMat = imagesMat;
    this->minWidth = minWidth;
    this->minHeight = minHeight;
}

bool ContentComparator::compareImages(const cv::Mat &image1, const cv::Mat &image2)
{
    if(!image1.data || !image2.data) return false;

    if(image1.rows < minWidth || image1.cols < minHeight ||
       image2.rows < minWidth || image2.cols < minHeight) {
        return false;
    }


    cv::Mat img1 = image1.clone();
    cv::Mat img2 = image2.clone();

    if(img1.rows != img2.rows || img1.cols != img2.cols) {

        // sprawdza czy proporcje sie za bardzo nie różnią
        float aspectRatio = (float) img1.rows / (float) img1.cols;
        int estimatedImg2Rows = img2.cols * aspectRatio;
        float rowsDiff = (float) estimatedImg2Rows / (float) img2.rows;
        if(rowsDiff > 2 || rowsDiff < 0.5f) {
            return false;
        }

        // skaluje większy do mniejszego
        if(img1.rows*img1.cols > img2.rows*img2.cols) {
            cv::resize(img1, img1, img2.size());
        } else {
            cv::resize(img2, img2, img1.size());
        }

    }

    cv::Mat bw(img1.size(), img1.type());
    cv::absdiff(img1, img2, img1);
    cv::threshold(img1, bw, 20, 255, cv::THRESH_BINARY);


    /*TODO: moze zamiast stalego tresholda dac reczne
    * kazdego pixela i jezeli sie roznia o np 10% to
    * wtedy ustawiac bialy, zamiast stalej wartosci roznicy
    */

    if(bw.channels() != 1) return false;
    if(bw.depth() != CV_8U) return false;

    if (!bw.isContinuous()) {
        qDebug() << "To sie nie powinno nigdy wyswietlic";
        bw = bw.clone();
        if(!bw.isContinuous()) return false;
    }

    int i, j;
    int pos;
    uchar *p;

    int n = bw.rows;
    int nRows = bw.rows;
    int nCols = bw.cols;
    int pCount = bw.rows*bw.cols;
    int minCCSize = pCount * 0.0005;
    int currentCCSize;
    int sumCCsize = 0;
    quint16 x;
    quint16 y;


    int detectionThresh = pCount * 0.01;

    FastPairQueue queue(2*detectionThresh);

    p = bw.ptr<uchar>(0);

    /*
    * Idzie po kolei po pixelach
    * Jezeli nie jest bialy to idz dalej
    * w przeciwnym razie  rozpoczyna procedure
    * wykrywania CC (connected component)
    * wykorzystujacy algorytm flood fill
    * procedura ta zamienia w CC biale piksele
    * na czarne i liczy ile ich bylo w danym CC
    *
    * Suma pikseli w CC dodawana jest do lacznej
    * sumy bialych pikseli na obrazku o ile
    * rozmiar CC nie byl mniejszy niz okreslone
    * minimum.
    *
    * Jezeli suma bialych pikseli przekroczyla
    * okreslony prog to cala pelta jest przerywana
    * bo oznacza to zbyt duze roznice miecy obrazkami
    *
    */

    for(i=1;i<nRows;++i) {
        for(j=1;j<nCols;++j) {

            if(p[i*n+j] != 255) continue;

            currentCCSize = 0;
            queue.clear();
            queue.enqueue(i, j);

            while(!queue.empty()) {
                queue.dequeue(x, y);
                pos = x*n+y;
                if(currentCCSize > detectionThresh) {
                    sumCCsize += currentCCSize;
                    break;
                }
                if(p[pos] == 255) {
                    ++currentCCSize;
                    p[pos] = 0;
                    if(y < nCols) queue.enqueue(x, y + 1);
                    if(x < nRows) queue.enqueue(x + 1, y);
                    if(y > 0) queue.enqueue(x, y - 1);
                    if(x > 0) queue.enqueue(x - 1, y);
                }
            }
            if(currentCCSize > minCCSize) {
                sumCCsize += currentCCSize;
            }
            if(sumCCsize > detectionThresh) {
                return false;
            }
        }
    }

    return true;
}


void ContentComparator::setImagesMat(QVector<cv::Mat> *imagesMat)
{
    this->imagesMat = imagesMat;
}

void ContentComparator::deleteImagesMat()
{
    delete imagesMat;
}

void ContentComparator::run()
{

    if(!imagesMat || !in) {
        return;
    }

    int i;
    int iCount = imagesMat->count();
    QString path;

    if(iCount < 1) return;

    bool isEmptyMatTab = true;
    for(i=0; i<iCount;i++) {
        if(imagesMat->at(i).data) {
            isEmptyMatTab = false;
            break;
        }
    }

    while(true) {
        try {
            path = in->dequeue();
        } catch (...) {
            // kolejka zostala wylaczana zatem konczymy watek
            return;
        }

        if(isEmptyMatTab) {
            reject(path);
            continue;
        }

        bool accepted = false;
        cv::Mat image = cv::imread(path.toUtf8().constData(),
                                   CV_LOAD_IMAGE_GRAYSCALE);

        try {
            for(i=0;i<iCount;i++) {

                if(compareImages(imagesMat->at(i), image)) {
                    accept(baseFiles->at(i).path, path, ContentComparator::Content);
                    accepted = true;
                    break;
                }
            }
        } catch(...) {
            ;
        }

        if(!accepted) {
            reject(path);
        }


    }
}
