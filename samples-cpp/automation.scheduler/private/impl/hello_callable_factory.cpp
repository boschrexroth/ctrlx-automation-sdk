/**
 * MIT License
 * 
 * Copyright (c) 2020-2021 Bosch Rexroth AG
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
 

// ---------------------------------------------------------------------------
// Include Files
// ---------------------------------------------------------------------------
#include "hello_callable_factory.h"

// ---------------------------------------------------------------------------
// Name Spaces
// ---------------------------------------------------------------------------
using namespace example;


// ---------------------------------------------------------------------------
// Declaration of constants
// ---------------------------------------------------------------------------


// ---------------------------------------------------------------------------
// Declaration of macros
// ---------------------------------------------------------------------------


// ---------------------------------------------------------------------------
// Forward function references
// ---------------------------------------------------------------------------


// ---------------------------------------------------------------------------
// Class implementation
// ---------------------------------------------------------------------------

HelloCallableFactory::HelloCallableFactory() : m_helloCallable(std::make_shared<HelloCallable>( ))
{
}

std::shared_ptr<common::scheduler::ICallable> HelloCallableFactory::createCallable(const comm::datalayer::Variant& param)
{
  std::cout << "create callable called 'helloCallable'" << std::endl;
  return m_helloCallable;
}

common::scheduler::SchedStatus HelloCallableFactory::destroyCallable(const std::shared_ptr<common::scheduler::ICallable>& callable)
{
  // Callable is destroyed from scheduler
  // shared pointer is deleted when factory is destroyed

  return common::scheduler::SchedStatus::SCHED_S_OK;
}

common::scheduler::SchedStatus HelloCallableFactory::getCallableArguments(std::vector<std::string>& arguments)
{
  arguments.push_back("");
  return common::scheduler::SchedStatus::SCHED_S_OK;
}

// ---------------------------------------------------------------------------
// C implementation
// ---------------------------------------------------------------------------
