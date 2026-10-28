class Fee {
private:
    int hourlyrate = 60;

public:
    Fee() = default;

    int getFees(int time) {
        int hours = (time + 3599) / 3600;
        return hours * hourlyrate;
    }
};

class Payment{
    public:
       virtual bool pay(int amount) = 0;
       virtual ~Payment() = default;
};

class CardPayment :  public Payment {
    public:
        bool pay(int amount) override {
            if(amount >=0){
                return true;
            }
            return false;
        }
};

class CashPayment :  public Payment {
    public:
        bool pay(int amount) override {
            if(amount >=0){
                return true;
            }
            return false;
        }
};