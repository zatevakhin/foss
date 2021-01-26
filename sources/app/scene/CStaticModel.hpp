#pragma once

#include "IModel.hpp"


class CStaticModel : public IModel
{

public:
    CStaticModel();
    ~CStaticModel();

    void draw(TProgramSharedPtr program) override;
    EModelType get_type() const override;
};
