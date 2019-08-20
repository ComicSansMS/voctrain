#include <central_widget.hpp>

#include <QKeyEvent>

#include <algorithm>
#include <iterator>

Q_A vocabulary[] = {
    {QString::fromUtf16(u"А а"), QString{"a"}},
    {QString::fromUtf16(u"Б б"), QString{"b"}},
    {QString::fromUtf16(u"В в"), QString{"v"}},
    {QString::fromUtf16(u"Г г"), QString{"h"}},
    {QString::fromUtf16(u"Ґ ґ"), QString{"g"}},
    {QString::fromUtf16(u"Д д"), QString{"d"}},
    {QString::fromUtf16(u"Е е"), QString{"e"}},
    {QString::fromUtf16(u"Є є"), QString{"je"}},
    {QString::fromUtf16(u"Ж ж"), QString{"sch"}},
    {QString::fromUtf16(u"З з"), QString{"s"}},
    {QString::fromUtf16(u"И и"), QString{"y"}},
    {QString::fromUtf16(u"І і"), QString{"i"}},
    {QString::fromUtf16(u"Ї ї"), QString{"ji"}},
    {QString::fromUtf16(u"Й й"), QString{"j"}},
    {QString::fromUtf16(u"К к"), QString{"k"}},
    {QString::fromUtf16(u"Л л"), QString{"l"}},
    {QString::fromUtf16(u"М м"), QString{"m"}},
    {QString::fromUtf16(u"Н н"), QString{"n"}},
    {QString::fromUtf16(u"О о"), QString{"o"}},
    {QString::fromUtf16(u"П п"), QString{"p"}},
    {QString::fromUtf16(u"Р р"), QString{"r"}},
    {QString::fromUtf16(u"С с"), QString{"s"}},
    {QString::fromUtf16(u"Т т"), QString{"t"}},
    {QString::fromUtf16(u"У у"), QString{"u"}},
    {QString::fromUtf16(u"Ф ф"), QString{"f"}},
    {QString::fromUtf16(u"Х х"), QString{"ch"}},
    {QString::fromUtf16(u"Ц ц"), QString{"z"}},
    {QString::fromUtf16(u"Ч ч"), QString{"tsch"}},
    {QString::fromUtf16(u"Ш ш"), QString{"sch"}},
    {QString::fromUtf16(u"Щ щ"), QString{"stsch"}},
    {QString::fromUtf16(u"ь"), QString{"-"}},
    {QString::fromUtf16(u"Ю ю"), QString{"ju"}},
    {QString::fromUtf16(u"Я я"), QString{"ja"}},
};
auto constexpr n_vocabulary = sizeof(vocabulary) / sizeof(vocabulary[0]);

CentralWidget::CentralWidget(QWidget* parent)
    :QWidget(parent),m_question(new QLabel("", this)), m_answer(new QLineEdit(this)),
     m_result(new QLabel("", this)), m_submit(new QPushButton("Start!", this)), m_rng(std::random_device()()),
     m_state(State::Init), m_currentVoc(0), m_hitLimit(3)
{
    m_layout.addWidget(m_question);
    m_layout.addWidget(m_answer);
    m_layout.addWidget(m_result);
    m_layout.addWidget(m_submit);
    setLayout(&m_layout);

    connect(m_submit, &QPushButton::clicked, this, &CentralWidget::onSubmit);
    connect(m_answer, &QLineEdit::returnPressed, this, &CentralWidget::onSubmit);

    initVocabulary();
}

void CentralWidget::onSubmit()
{

    switch (m_state) {
    case State::Init:
        /* fallthrough */
    case State::Guess:
        newGuess();
        m_state = State::Reveal;
        break;
    case State::Reveal:
        reveal();
        m_state = isDone() ? State::Done : State::Guess;
        break;
    case State::Done:
        m_submit->setText("Done!");
        break;
    }
}

void CentralWidget::newGuess()
{
    std::uniform_int_distribution<std::size_t> dist(0, n_vocabulary - 1);
    do {
        m_currentVoc = dist(m_rng);
    } while (m_hits[m_currentVoc] >= m_hitLimit);
    m_question->setText(vocabulary[m_currentVoc].q);
    m_answer->setText("");
    m_answer->setEnabled(true);
    m_result->setText("");
    m_submit->setText("Submit");
    m_answer->setFocus();
}

void CentralWidget::reveal()
{
    m_answer->setEnabled(false);
    if (m_answer->text().toLower() == vocabulary[m_currentVoc].a) {
        m_result->setText("Correct!");
        ++m_hits[m_currentVoc];
    } else {
        m_result->setText(vocabulary[m_currentVoc].a);
        ++m_misses[m_currentVoc];
    }
    m_submit->setText("Next");
    m_submit->setFocus();
}

bool CentralWidget::isDone()
{
    return std::all_of(std::begin(m_hits), std::end(m_hits), [limit = m_hitLimit](int i) { return i >= limit; });
}

void CentralWidget::initVocabulary()
{
    std::copy(std::begin(vocabulary), std::end(vocabulary), std::back_inserter(m_vocabulary));
    m_hits.resize(m_vocabulary.size(), 0);
    m_misses.resize(m_vocabulary.size(), 0);
}
