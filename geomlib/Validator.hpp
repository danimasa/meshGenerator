#pragma once

class Validator {
public:
    virtual bool Validate(T object) = 0;
};