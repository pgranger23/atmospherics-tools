#pragma once
#include <string>
#include "FluxManager.h"
#include "Reader.h"
#include "Writer.h"

template<typename T>
class Calculator
{
private:
    const FluxManager& _mgr;
    Reader<T>& _rdr;
    Writer& _wrt;
    float _exposure_scaling;

    Data<double> FetchData();

public:
    Calculator(const FluxManager& mgr, Reader<T>& rdr, Writer& wrt, float exposure_scaling);
    ~Calculator();
    void Process();
};
