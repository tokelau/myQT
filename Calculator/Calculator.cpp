// ======================================================================
//  Calculator.cpp
// ======================================================================
//                   This file is a part of the book
//             "Qt 5.10 Professional programming with C++"
//                       http://qt-book.com
// ======================================================================
//  Copyright (c) 2017 by Max Schlee
// ======================================================================

#include <QtWidgets>
#include <cmath>
#include "Calculator.h"
#include <iostream>

// ----------------------------------------------------------------------
Calculator::Calculator(QWidget* pwgt/*= 0*/) : QWidget(pwgt)
{
    m_plcd = new QLCDNumber(12);
    m_plcd->setSegmentStyle(QLCDNumber::Flat);
    m_plcd->setMinimumSize(150, 50);

    QChar aButtons[4][4] = {{'7', '8', '9', '/'},
                            {'4', '5', '6', '*'},
                            {'1', '2', '3', '-'},
                            {'0', '.', '=', '+'}
                           };

    //Layout setup
    QGridLayout* ptopLayout = new QGridLayout;
    ptopLayout->addWidget(m_plcd, 0, 0, 1, 3);
    ptopLayout->addWidget(createButton("SIN"), 1, 0);
    ptopLayout->addWidget(createButton("COS"), 1, 1);
    ptopLayout->addWidget(createButton("1/x"), 1, 2);
    ptopLayout->addWidget(createButton("TAN"), 1, 3);
    ptopLayout->addWidget(createButton("CE"), 0, 3);

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
           ptopLayout->addWidget(createButton(aButtons[i][j]), i + 2, j);
        }
    }
    setLayout(ptopLayout);
}

// ----------------------------------------------------------------------
QPushButton* Calculator::createButton(const QString& str)
{
    QPushButton* pcmd = new QPushButton(str);
    pcmd->setMinimumSize(40, 40);
    connect(pcmd, SIGNAL(clicked()), SLOT(slotButtonClicked()));
    return pcmd;
}


// ----------------------------------------------------------------------
void Calculator::calculate()
{
    if (m_stk.count() == 2) {
        QString strOperation = m_stk.pop();
        qreal   fOperand1    = m_stk.pop().toDouble();
        qreal   fResult      = 0;

        if (strOperation == "SIN") {
            fResult = sin(fOperand1*3.14159265/180.0);
        }
        if (strOperation == "COS") {
            fResult = cos(fOperand1*3.14159265/180.0);
        }
        if (strOperation == "TAN") {
            fResult = tan(fOperand1*3.14159265/180.0);
        }
        if (strOperation == "1/x") {
            fResult = 1/fOperand1;
        }

        m_plcd->display(fResult);
    } else if (m_stk.count() == 3) {
        qreal   fOperand2    = m_stk.pop().toDouble();
        QString strOperation = m_stk.pop();
        qreal   fOperand1    = m_stk.pop().toDouble();
        qreal   fResult      = 0;

        if (strOperation == "+") {
            fResult = fOperand1 + fOperand2;
        }
        if (strOperation == "-") {
            fResult = fOperand1 - fOperand2;
        }
        if (strOperation == "/") {
            fResult = fOperand1 / fOperand2;
        }
        if (strOperation == "*") {
            fResult = fOperand1 * fOperand2;
        }
        m_plcd->display(fResult);
    }
}

// ----------------------------------------------------------------------
void Calculator::slotButtonClicked()
{
    QString str = ((QPushButton*)sender())->text();

    if (str == "CE") {
        m_stk.clear();
        m_strDisplay = "";
        m_plcd->display("0");
        return;
    }
    if (str.contains(QRegExp("[0-9]")) && !str.contains(QRegExp("1/x"))) {
        m_strDisplay += str;
        m_plcd->display(m_strDisplay.toDouble());
    }
    else if (str == ".") {
        m_strDisplay += str;
        m_plcd->display(m_strDisplay);
    }
    else {
        if (str == "SIN" || str == "COS" || str == "1/x" || str == "TAN") {
            if (!m_stk.count() > 1) {
                m_stk.push(QString().setNum(m_plcd->value()));
                calculate();
                m_stk.clear();
                m_stk.push(QString().setNum(m_plcd->value()));
            } else if (m_stk.empty()) {
                m_stk.push(QString().setNum(m_plcd->value()));
            }
            m_stk.push(str);
            calculate();

            m_stk.clear();
            //m_stk.push(QString().setNum(m_plcd->value()));

        } else if (m_stk.count() == 2) {
            //std::cout<<"2"<<std::endl;
            m_stk.push(QString().setNum(m_plcd->value()));
            calculate();
            m_stk.clear();
            m_stk.push(QString().setNum(m_plcd->value()));
            if (str != "=") {
                m_stk.push(str);
            }
            m_strDisplay = "";
        } else {
            m_stk.clear();
            m_stk.push(QString().setNum(m_plcd->value()));
            if (str != "=") {
                m_stk.push(str);
            }
            m_strDisplay = "";
        }
    }
}
