#include <iostream>
#include <vector>
#include <memory>
#include <stdexcept>
#include <limits>
#include <locale>

class IJob {
protected:
    double basePay;
public:
    IJob(double basePay) : basePay(basePay) {
        if (basePay < 0) throw std::invalid_argument("Базовая оплата не может быть отрицательной.");
    }

    virtual double calculatePay() const = 0;
    virtual ~IJob() = default;

    void setBasePay(double newBasePay) {
        if (newBasePay < 0) throw std::invalid_argument("Базовая оплата не может быть отрицательной.");
        basePay = newBasePay;
    }

    double getBasePay() const {
        return basePay;
    }
};

class RegularJob : public IJob {
public:
    RegularJob(double basePay) : IJob(basePay) {}

    double calculatePay() const override {
        return basePay;
    }
};

class BonusJob : public IJob {
private:
    double bonusRate;
public:
    BonusJob(double basePay, double bonusRate = 0) : IJob(basePay), bonusRate(bonusRate / 100) {
        if (bonusRate < 0 || bonusRate > 100)
            throw std::invalid_argument("Некорректный процент надбавки. Укажите значение от 0 до 100.");
    }

    double calculatePay() const override {
        return basePay * (1 + bonusRate);
    }

    void setBonusRate(double newBonusRate) {
        if (newBonusRate < 0 || newBonusRate > 100)
            throw std::invalid_argument("Некорректный процент надбавки. Укажите значение от 0 до 100.");
        bonusRate = newBonusRate / 100;
    }

    double getBonusRate() const {
        return bonusRate * 100;
    }
};

class PayrollDepartment {
private:
    std::vector<std::shared_ptr<IJob>> jobs;
public:
    void addRegularJob(double basePay) {
        jobs.push_back(std::make_shared<RegularJob>(basePay));
    }

    void addBonusJob(double basePay, double bonusRate) {
        jobs.push_back(std::make_shared<BonusJob>(basePay, bonusRate));
    }

    double calculateAveragePay() const {
        if (jobs.empty()) throw std::runtime_error("Нет данных о работах для расчета средней оплаты.");
        double totalPay = 0;
        for (const auto& job : jobs) {
            totalPay += job->calculatePay();
        }
        return totalPay / jobs.size();
    }

    void displayJobsInfo() const {
        if (jobs.empty()) {
            std::cout << "Данные о работах отсутствуют.\n";
            return;
        }

        std::cout << "Информация о работах:\n";
        for (size_t i = 0; i < jobs.size(); ++i) {
            const auto& job = jobs[i];
            std::cout << "Работа " << i + 1 << ": базовая оплата = " << job->getBasePay()
                << ", итоговая оплата = " << job->calculatePay() << "\n";
        }
    }
};

void displayMenu() {
    std::cout << "\nМеню:\n";
    std::cout << "1. Добавить обычную работу\n";
    std::cout << "2. Добавить работу с надбавкой\n";
    std::cout << "3. Рассчитать среднюю оплату\n";
    std::cout << "4. Показать информацию о работах\n";
    std::cout << "0. Выход\n";
    std::cout << "Выберите пункт: ";
}

void addRegularJob(PayrollDepartment& department) {
    double basePay;
    std::cout << "Введите базовую оплату для обычной работы: ";
    std::cin >> basePay;
    try {
        department.addRegularJob(basePay);
        std::cout << "Обычная работа добавлена.\n";
    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
    }
}

void addBonusJob(PayrollDepartment& department) {
    double basePay, bonusRate;
    std::cout << "Введите базовую оплату для работы с надбавкой: ";
    std::cin >> basePay;
    std::cout << "Введите процент надбавки (0-100): ";
    std::cin >> bonusRate;
    try {
        department.addBonusJob(basePay, bonusRate);
        std::cout << "Работа с надбавкой добавлена.\n";
    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
    }
}

void calculateAveragePay(const PayrollDepartment& department) {
    try {
        double avgPay = department.calculateAveragePay();
        std::cout << "Средняя оплата: " << avgPay << " руб.\n";
    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
    }
}

void displayJobsInfo(const PayrollDepartment& department) {
    department.displayJobsInfo();
}

int main() {
    setlocale(LC_ALL, "Russian");
    PayrollDepartment department;
    int choice;

    do {
        displayMenu();
        std::cin >> choice;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cerr << "Некорректный ввод. Попробуйте снова.\n";
            continue;
        }

        switch (choice) {
        case 1:
            addRegularJob(department);
            break;
        case 2:
            addBonusJob(department);
            break;
        case 3:
            calculateAveragePay(department);
            break;
        case 4:
            displayJobsInfo(department);
            break;
        case 0:
            std::cout << "Выход из программы.\n";
            break;
        default:
            std::cerr << "Некорректный выбор. Попробуйте снова.\n";
            break;
        }
    } while (choice != 0);

    return 0;
}
