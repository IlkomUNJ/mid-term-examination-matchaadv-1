#include "drawingcanvas.h"
#include <iostream>
#include <QPainter>
#include <QPen>
#include <QColor>

static bool isSegmentPattern(const bool win[3][3])
{
    int count = 0;
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            if (win[i][j])
                ++count;

    if (count != 3)
        return false;

    // Horizontal
    if (win[1][0] && win[1][1] && win[1][2])
        return true;
    // Vertikal
    if (win[0][1] && win[1][1] && win[2][1])
        return true;
    // Diagonal ↘
    if (win[0][0] && win[1][1] && win[2][2])
        return true;
    // Diagonal ↙
    if (win[0][2] && win[1][1] && win[2][0])
        return true;

    return false;
}

DrawingCanvas::DrawingCanvas(QWidget *parent)
{
    // Set a minimum size for the canvas
    setMinimumSize(this->WINDOW_WIDTH, this->WINDOW_HEIGHT);
    // Set a solid background color
    setStyleSheet("background-color: white; border: 1px solid gray;");
}

void DrawingCanvas::clearPoints()
{
    m_points.clear();
    m_candidates.clear();
    // Trigger a repaint to clear the canvas
    update();
}

void DrawingCanvas::paintLines()
{
    /* Todo
     * Implement lines drawing per even pair
     */

    isPaintLinesClicked = true;
    update();
}

void DrawingCanvas::segmentDetection()
{
    QPixmap pixmap = this->grab();
    QImage image = pixmap.toImage();

    // Reset daftar kandidat sebelumnya
    m_candidates.clear();

    // Sliding window 3x3 (abaikan tepi 1 piksel)
    for (int i = 1; i < image.width() - 1; ++i)
    {
        for (int j = 1; j < image.height() - 1; ++j)
        {
            bool local_window[3][3] = {{false}};

            // Isi jendela lokal dari citra
            for (int m = -1; m <= 1; ++m)
            {
                for (int n = -1; n <= 1; ++n)
                {
                    QRgb rgbValue = image.pixel(i + m, j + n);
                    // Piksel aktif = bukan putih (0xffffffff)
                    local_window[m + 1][n + 1] = (rgbValue != 0xffffffff);
                }
            }

            // Periksa apakah jendela ini cocok dengan pola segmen
            if (isSegmentPattern(local_window))
            {
                m_candidates.append(QPoint(i, j)); // simpan titik tengah
            }
        }
    }

    // Trigger repaint untuk menggambar kotak ungu
    update();
}

void DrawingCanvas::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Set up the pen and brush for drawing the points
    QPen pen(Qt::blue, 5);
    painter.setPen(pen);
    painter.setBrush(QBrush(Qt::blue));

    // Draw a small circle at each stored point
    for (const QPoint &point : std::as_const(m_points))
    {
        painter.drawEllipse(point, 3, 3);
    }

    if (isPaintLinesClicked)
    {
        std::cout << "paint lines block is called" << std::endl;
        pen.setColor(Qt::red);
        pen.setWidth(4); // 4-pixel wide line
        pen.setStyle(Qt::SolidLine);
        painter.setPen(pen);

        for (int i = 0; i < m_points.size() - 1; i += 2)
        {
            painter.drawLine(m_points[i], m_points[i + 1]);
        }
        isPaintLinesClicked = false;

        // return painter pen to blue
        pen.setColor(Qt::blue);
        painter.setPen(pen);
    }

    // ✅ GAMBAR KOTAK UNGU UNTUK SETIAP KANDIDAT SEGMENT
    QPen purplePen(QColor(128, 0, 128), 2); // warna ungu
    painter.setPen(purplePen);
    painter.setBrush(Qt::NoBrush); // hanya outline
    for (const QPoint &center : std::as_const(m_candidates))
    {
        // Gambar kotak 4x4 berpusat di (center.x, center.y)
        painter.drawRect(center.x() - 2, center.y() - 2, 4, 4);
    }
}

void DrawingCanvas::mousePressEvent(QMouseEvent *event)
{
    // Add the mouse click position to our vector of points
    m_points.append(event->pos());
    // Trigger a repaint
    update();
}