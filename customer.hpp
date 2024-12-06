class Customer{
    private:
        string name;
        string email;
        string phone;
    public:
        // Constructor
        Customer(string name, string email, string phone) : name(name), email(email), phone(phone) {}
        
        // Getters
        const string& getName() {return name; }
        const string& getEmail() {return email; }
        const string& getPhone() {return phone; }

        json toJSON() const {
            return {
                {"name", name},
                {"email", email},
                {"phone", phone}
            };
        }

        static Customer fromJSON(const json& j) {
            return Customer(j["name"], j["email"], j["phone"]);
        }

        // Display customer details
        void display() const {
            cout << "Name: " << name << ", Email: " << email << ", Phone: " << phone << endl;
        }

};