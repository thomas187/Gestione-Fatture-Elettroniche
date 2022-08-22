#include "singleshot.h"

Singleshot::Singleshot()
{

}

Singleshot::~Singleshot()
{
    core::timer::stop(t_singleShot);
}
