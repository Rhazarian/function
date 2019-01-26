#ifndef FUNCTION_H
#define FUNCTION_H

#include <cstddef>
#include <cstring>
#include <functional>

template<typename HelperT>
struct function;

template<typename RetT, typename ...ArgTs>
struct function<RetT(ArgTs...)> {
public:
    function() noexcept;
    function(std::nullptr_t) noexcept;
    function(function const& other);
    function(function&& other) noexcept;
    template<typename FuncT>
    function(FuncT func);
    ~function();
    function& operator=(function const& other);
    function& operator=(function&& other) noexcept;
    void swap(function& other) noexcept;
    explicit operator bool() const noexcept;
    RetT operator()(ArgTs... args) const;
private:
    struct func_holder {
        func_holder() = default;
        virtual ~func_holder() = default;
        virtual RetT operator()(ArgTs&&... args) const = 0;
        virtual func_holder* make_copy() const = 0;
    };

    template<typename FuncT>
    struct func_t_holder : func_holder {
        explicit func_t_holder(FuncT&& func);
        explicit func_t_holder(FuncT const& func);
        ~func_t_holder() override = default;
        RetT operator()(ArgTs&&... args) const override;
        func_t_holder* make_copy() const override;
    private:
        FuncT func;
    };

    static constexpr auto mem_size = 16;
    union {
        mutable char mem[mem_size]{};
        func_holder* ptr;
    };
    bool in_place = false;
};

template<typename RetT, typename... ArgTs>
template<typename FuncT>
function<RetT(ArgTs...)>::func_t_holder<FuncT>::func_t_holder(FuncT&& func) : func(std::move(func)) { }

template<typename RetT, typename... ArgTs>
template<typename FuncT>
function<RetT(ArgTs...)>::func_t_holder<FuncT>::func_t_holder(FuncT const& func) : func(func) { }

template<typename RetT, typename... ArgTs>
template<typename FuncT>
RetT function<RetT(ArgTs...)>::func_t_holder<FuncT>::operator()(ArgTs&&... args) const
{
    return func(std::forward<ArgTs>(args)...);
}

template<typename RetT, typename... ArgTs>
template<typename FuncT>
typename function<RetT(ArgTs...)>::template func_t_holder<FuncT>* function<RetT(ArgTs...)>::func_t_holder<FuncT>::make_copy() const
{
    return new func_t_holder(func);
}

template<typename RetT, typename... ArgTs>
function<RetT(ArgTs...)>::function() noexcept {
    ptr = nullptr;
};

template<typename RetT, typename... ArgTs>
function<RetT(ArgTs...)>::function(std::nullptr_t) noexcept : function() {
    ptr = nullptr;
}

template<typename RetT, typename... ArgTs>
function<RetT(ArgTs...)>::function(function const& other)
{
    in_place = other.in_place;
    if (in_place) {
        memcpy(mem, other.mem, mem_size);
    } else {
        ptr = other.ptr->make_copy();
    }
}

template<typename RetT, typename... ArgTs>
function<RetT(ArgTs...)>::function(function&& other) noexcept
{
    ptr = nullptr;
    in_place = false;
    swap(other);
}

template<typename RetT, typename... ArgTs>
template<typename FuncT>
function<RetT(ArgTs...)>::function(FuncT func)
{
    in_place = sizeof(decltype(func_t_holder<FuncT>(func))) <= mem_size;
    if (in_place) {
        new (mem) func_t_holder<FuncT>(std::move(func));
    } else {
        ptr = new func_t_holder<FuncT>(std::move(func));
    }
}

template<typename RetT, typename... ArgTs>
function<RetT(ArgTs...)>::~function()
{
    if (in_place) {
        reinterpret_cast<func_holder*>(mem)->~func_holder();
    } else {
        delete ptr;
    }
}

template<typename RetT, typename... ArgTs>
function<RetT(ArgTs...)>& function<RetT(ArgTs...)>::operator=(function<RetT(ArgTs...)> const& other)
{
    function<RetT(ArgTs...)> copy(other);
    swap(copy);
    return *this;
}

template<typename RetT, typename... ArgTs>
function<RetT(ArgTs...)>& function<RetT(ArgTs...)>::operator=(function<RetT(ArgTs...)>&& other) noexcept
{
    swap(other);
    return *this;
}

template<typename RetT, typename... ArgTs>
void function<RetT(ArgTs...)>::swap(function& other) noexcept
{
    std::swap(mem, other.mem);
    std::swap(in_place, other.in_place);
}

template<typename RetT, typename... ArgTs>
function<RetT(ArgTs...)>::operator bool() const noexcept
{
    return (in_place || ptr != nullptr);
}

template<typename RetT, typename... ArgTs>
RetT function<RetT(ArgTs...)>::operator()(ArgTs... args) const
{
    if (!(*this)) {
        throw std::bad_function_call();
    }
    if (in_place) {
        return reinterpret_cast<func_holder*>(mem)->operator()(std::forward<ArgTs>(args)...);
    } else {
        return ptr->operator()(std::forward<ArgTs>(args)...);
    }
}

#endif //FUNCTION_H
