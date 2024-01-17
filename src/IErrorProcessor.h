#pragma once
#include <string>
#include <memory>

class IErrorProcessor
{
public:
    using Ptr = std::shared_ptr<IErrorProcessor>;

    virtual ~IErrorProcessor() = default;
    virtual void error(const std::string& msg) = 0;
};
