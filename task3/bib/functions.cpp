#include "functions.hpp"

class FunFactory::FunImpl{
    class AbstrFunCreator{
    public:
        virtual ~AbstrFunCreator(){}
        virtual std::unique_ptr<TFunction> create() const = 0;
    };
    using FunCreatorPtr = std::shared_ptr<AbstrFunCreator>;
    using FunRegCreators = std::unordered_map<std::string, FunCreatorPtr>;
    FunRegCreators registered_creators;
public:
    template <class CurrentFun>
    class FunCreator : public AbstrFunCreator{
        std::unique_ptr<TFunction> create() const override{
            return std::make_unique<CurrentFun>();
        }
    };

    FunImpl(){reg_all();}
    template <typename FunType>
    void reg_creator(const std::string& name){
        registered_creators[name] = std::make_shared<FunCreator<FunType>>();
    }

    void reg_all(){
        reg_creator<Constant>("constant");
    }


    std::unique_ptr<TFunction> create_fun(const std::string& name) const{
        auto creator = registered_creators.find(name);
        if (creator == registered_creators.end()){
            return nullptr;
        }
        return creator->second->create();
    }
};

FunFactory::FunFactory() : Impl(std::make_unique<FunFactory::FunImpl>()){}
FunFactory::~FunFactory(){}
std::unique_ptr<TFunction> FunFactory::CreateFunction(const std::string& name) const{
    return Impl->create_fun(name);
}