#include "functions.hpp"

class FunFactory::FunImpl{
    class AbstrFunCreator{
    public:
        virtual ~AbstrFunCreator(){}
        virtual std::shared_ptr<TFunction> create(const std::vector<double> koefs) const = 0;
    };
    using FunCreatorPtr = std::shared_ptr<AbstrFunCreator>;
    using FunRegCreators = std::unordered_map<std::string, FunCreatorPtr>;
    FunRegCreators registered_creators;
public:
    template <class CurrentFun>
    class FunCreator : public AbstrFunCreator{
        std::shared_ptr<TFunction> create(const std::vector<double> koefs) const override{
            return std::make_shared<CurrentFun>(koefs);
        }
    };

    FunImpl(){reg_all();}
    template <typename FunType>
    void reg_creator(const std::string& name){
        registered_creators[name] = std::make_shared<FunCreator<FunType>>();
    }

    void reg_all(){
        reg_creator<Constant>("constant");
        reg_creator<Identical>("identical");
        reg_creator<Sedate>("sedate");
        reg_creator<Exp>("exp");
        reg_creator<Polynomial>("polynomial");
    }


    std::shared_ptr<TFunction> create_fun(const std::string& name, const std::vector<double> koefs) const{
        auto creator = registered_creators.find(name);
        if (creator == registered_creators.end()){
            return nullptr;
        }
        return creator->second->create(koefs);
    }
};

FunFactory::FunFactory() : Impl(std::make_unique<FunFactory::FunImpl>()){}
FunFactory::~FunFactory(){}
std::shared_ptr<TFunction> FunFactory::CreateFunction(const std::string& name,  const std::vector<double> koefs) const{
    return Impl->create_fun(name, koefs);
}

TFunPtr operator+ (TFunPtr left, TFunPtr right){
    return std::make_shared<Addition>(left->copy(), right->copy(), '+');
}

TFunPtr operator- (TFunPtr left, TFunPtr right){
    return std::make_shared<Addition>(left->copy(), right->copy(), '-');
}

TFunPtr operator* (TFunPtr left, TFunPtr right){
    return std::make_shared<Addition>(left->copy(), right->copy(), '*');
}

TFunPtr operator/ (TFunPtr left, TFunPtr right){
    return std::make_shared<Addition>(left->copy(), right->copy(), '/');
}

double equation(TFunPtr f, double step_size, double steps, double start){
    auto f_squared = f * f;
    double prev_x = start, cur_x = 0.1;
    for (int t = 0; t < steps; t++){
        cur_x = prev_x - step_size * f_squared->GetDeriv(prev_x);
        prev_x = cur_x;
    }
    return cur_x;

}