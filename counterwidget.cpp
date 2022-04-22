#include <QHBoxLayout>
#include "counterwidget.h"
#include "config.h"

CounterWidget::CounterWidget(QWidget *parent)
    : QWidget(parent), m_count(0), m_timer(this)
{
    setWindowTitle("BinaryCounter");
    resize(300, 200);
    QHBoxLayout* layout = new QHBoxLayout(this);
    setLayout(layout);

    // QLCDNumber zur Anzeige des Zaehlerstandes dezimal:
    m_lcd = new QLCDNumber(2);
    layout->addWidget(m_lcd);
    m_lcd->display(m_count);

    m_gpio = new Gpio();

    connect(&m_timer, &QTimer::timeout, this, &CounterWidget::updateCounter);
    m_timer.start(T_UPDATE);
}

CounterWidget::~CounterWidget()
{

}

void CounterWidget::updateLEDs()
{
    for(int i = 0; i < 4; i++)
       m_gpio->set(LEDS[i], (m_count >> i) & 0b1); //numbers to binary
}

// Zaehlerstand im Widget und dual ueber LEDs anzeigen:
void CounterWidget::updateCounter()
{
    if(m_gpio->isActivated(BUTTONS[2]))
    {
        m_count = (m_count + 1) & 0xF; //0xF means Hexadecimal
    }
    else if(m_gpio->isActivated(BUTTONS[0]))
    {
         m_count = (m_count - 1) & 0xF;
    }
    else if(m_gpio->isActivated(BUTTONS[1]))
    {
         m_count = 0;
    }
    m_lcd->display(m_count);
    updateLEDs();
}
