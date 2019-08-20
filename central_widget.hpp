#ifndef INCLUDE_GUARD_CENTRAL_WIDGET_HPP
#define INCLUDE_GUARD_CENTRAL_WIDGET_HPP

#include <QWidget>

#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QString>
#include <QVBoxLayout>

#include <random>
#include <vector>

struct Q_A {
    QString q;
    QString a;
};

class CentralWidget : public QWidget
{
    Q_OBJECT;
private:
    QVBoxLayout m_layout;
    QLabel* m_question;
    QLineEdit* m_answer;
    QLabel* m_result;
    QPushButton* m_submit;
    std::mt19937 m_rng;
    enum class State {
        Init,
        Guess,
        Reveal,
        Done
    } m_state;
    std::size_t m_currentVoc;
    int m_hitLimit;
    std::vector<Q_A> m_vocabulary;
    std::vector<int> m_hits;
    std::vector<int> m_misses;
public:
    CentralWidget(QWidget* parent);

public slots:
    void onSubmit();
private:
    void initVocabulary();
    void newGuess();
    void reveal();
    bool isDone();
};

#endif
