#pragma once

struct ID_CORE
{
    unsigned int ID=0;
    bool isValid() const {return ID != 0;}
    bool isInvalid() const {return ID == 0;}
};