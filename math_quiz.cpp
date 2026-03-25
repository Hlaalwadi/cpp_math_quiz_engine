#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <limits>

using namespace std;

int randomInt(int From, int To)
{
    return rand() % (To - From + 1) + From;
}

enum enLevel { easy = 0, hard = 1, mid = 2, mix = 3 };
enum enOpType { opAdd = 0, opSub = 1, opMul = 2, opDiv = 3 };

struct stQuestion
{
    int number1 = 0;
    int number2 = 0;
    enLevel Quiestionlevel = enLevel::mix;
    enOpType opType = enOpType::opAdd;
    int Answer = 0;
    int userAnswer = 0;
    bool Result = true;
};

struct stQuiz
{
    int numberOfquestions = 0;
    stQuestion questions[100];
    enLevel Quizlevel = enLevel::easy;
    enOpType QuizOp = enOpType::opAdd;
    int CorrectAnswers = 0;
    bool QuizResults = true;
};

enLevel QuizLevel(int number)
{
    switch (number)
    {
    case 0:
        return enLevel::easy;
    case 1:
        return enLevel::hard;
    case 2:
        return enLevel::mid;
    case 3:
        return enLevel::mix;
    default:
        return enLevel::mix;
    }
}

enOpType QuizOpType(int number)
{
    switch (number)
    {
    case 0:
        return enOpType::opAdd;
    case 1:
        return enOpType::opSub;
    case 2:
        return enOpType::opMul;
    case 3:
        return enOpType::opDiv;
    default:
        return enOpType::opAdd;
    }
}

int AskForValue(const string& message, int From, int To)
{
    int number;
    while (true)
    {
        cout << message << endl;
        if (cin >> number)
        {
            if (number >= From && number <= To)
                return number;
        }
        else
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        cout << "Please enter a number between " << From << " and " << To << "." << endl;
    }
}

char getOpprint(enOpType opType)
{
    char op[] = { '+', '-', '*', '/' };
    return op[(int)opType];
}

int DoOperation(enOpType op, int a, int b)
{
    switch (op)
    {
    case enOpType::opAdd:
        return a + b;
    case enOpType::opSub:
        return a - b;
    case enOpType::opMul:
        return a * b;
    case enOpType::opDiv:
        if (b == 0) return 0; // defensive; b should not be zero due to generation logic
        return a / b;
    default:
        return 0;
    }
}

string tab(int a)
{
    string word;
    for (int i = 0; i < a; i++)
    {
        word += '\t';
    }
    return word;
}

int numberlevel(enLevel level)
{
    switch (level)
    {
    case enLevel::easy:
        return randomInt(1, 10);
    case enLevel::mid:
        return randomInt(10, 100);
    case enLevel::hard:
        return randomInt(100, 1000);
    case enLevel::mix:
        return randomInt(1, 1000);
    default:
        return 0;
    }
}

stQuestion Question(enOpType op, enLevel level)
{
    stQuestion Question;
    Question.opType = op;
    Question.Quiestionlevel = level;

    Question.number1 = numberlevel(Question.Quiestionlevel);
    Question.number2 = numberlevel(Question.Quiestionlevel);

    // Ensure second number is not zero when dividing
    if (Question.opType == enOpType::opDiv)
    {
        while (Question.number2 == 0)
            Question.number2 = numberlevel(Question.Quiestionlevel);
    }

    cout << tab(1) << Question.number1 << endl;
    cout << tab(1) << Question.number2 << endl;

    cout << tab(3) << getOpprint(Question.opType) << endl;
    cout << "---------" << endl;

    Question.Answer = DoOperation(Question.opType, Question.number1, Question.number2);

    // read user answer safely
    while (!(cin >> Question.userAnswer))
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Please enter a valid integer answer: ";
    }

    if (Question.userAnswer == Question.Answer)
    {
        Question.Result = true;
        cout << "Correct Answer !!" << endl;
    }
    else
    {
        Question.Result = false;
        cout << "Wrong Answer !! (Correct: " << Question.Answer << ")" << endl;
    }

    return Question;
}

void changeColor(bool result)
{
#ifdef _WIN32
    if (result)
    {
        system("color 25");
    }
    else
    {
        cout << "\a";
        system("color 4F");
    }
#endif
}

void PrintMathGameOver()
{
    cout << tab(4) << "---------------------Math Game over---------------------" << endl;
    cout << tab(4) << "--------------------------------------------------------" << endl;
}

void PrintResults(stQuiz& Quiz)
{
    PrintMathGameOver();

    cout << tab(4) << "Quiz Results : " << endl;
    cout << tab(4) << "Number OF Questions : " << Quiz.numberOfquestions << endl;
    cout << tab(4) << "Correct Answers : " << Quiz.CorrectAnswers << endl;

    if (Quiz.CorrectAnswers > (Quiz.numberOfquestions - Quiz.CorrectAnswers))
    {
        cout << tab(4) << "PASS !! =) " << endl;
        Quiz.QuizResults = true;
    }
    else
    {
        cout << tab(4) << "FAIL !! =( " << endl;
        Quiz.QuizResults = false;
    }
    cout << tab(4) << "--------------------------------------------------------" << endl;
}

void Quiz()
{
    stQuiz Quiz;

    Quiz.numberOfquestions = AskForValue("How many questions do you want (1-100): ", 1, 100);

    int level = AskForValue("What's the quiz level you want : (easy = 0, hard = 1, mid = 2, mix = 3) : ", 0, 3);
    Quiz.Quizlevel = QuizLevel(level);

    int op = AskForValue("What's the quiz operation type you want : (add=0, sub=1, mul=2, div=3, mix=4) : ", 0, 4);
    int Quizop = 0;

    for (int i = 0; i < Quiz.numberOfquestions; i++)
    {
        cout << "Question " << (i + 1) << endl;
        if (op == 4)
        {
            Quizop = randomInt(0, 3);
        }
        else
        {
            Quizop = op;
        }

        Quiz.QuizOp = QuizOpType(Quizop);

        Quiz.questions[i] = Question(Quiz.QuizOp, Quiz.Quizlevel);

        if (Quiz.questions[i].Result)
        {
            Quiz.CorrectAnswers++;
        }

        changeColor(Quiz.questions[i].Result);
    }

    PrintResults(Quiz);
}

int main()
{
    srand(static_cast<unsigned>(time(NULL)));

    Quiz();
    return 0;
}