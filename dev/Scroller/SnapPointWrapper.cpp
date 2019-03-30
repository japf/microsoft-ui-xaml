// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include "pch.h"
#include "common.h"
#include "SnapPointWrapper.h"

template<typename T>
SnapPointWrapper<T>::SnapPointWrapper(T const& snapPoint)
    : m_snapPoint(snapPoint)
{
}

#ifdef _DEBUG
template<typename T>
SnapPointWrapper<T>::~SnapPointWrapper()
{
}
#endif //_DEBUG

template<typename T>
T SnapPointWrapper<T>::SnapPoint() const
{
    return m_snapPoint;
}

template<typename T>
std::tuple<double, double> SnapPointWrapper<T>::ActualApplicableZone() const
{
    return m_actualApplicableZone;
}

template<typename T>
int SnapPointWrapper<T>::CombinationCount() const
{
    return m_combinationCount;
}

template<typename T>
bool SnapPointWrapper<T>::ResetIgnoredValue()
{
    if (!isnan(m_ignoredValue))
    {
        m_ignoredValue = NAN;
        return true;
    }

    return false;
}

template<typename T>
void SnapPointWrapper<T>::SetIgnoredValue(double ignoredValue)
{
    MUX_ASSERT(!isnan(ignoredValue));

    m_ignoredValue = ignoredValue;
}

template<typename T>
winrt::ExpressionAnimation SnapPointWrapper<T>::CreateRestingPointExpression(
    winrt::Compositor const& compositor,
    winrt::hstring const& target,
    winrt::hstring const& scale)
{
    winrt::SnapPointBase winrtSnapPoint = safe_cast<winrt::SnapPointBase>(m_snapPoint);
    SnapPointBase* snapPoint = winrt::get_self<SnapPointBase>(winrtSnapPoint);

    return snapPoint->CreateRestingPointExpression(
        m_subExpressionsPropertySet,
        m_ignoredValue,
        m_actualImpulseApplicableZone,
        compositor,
        target,
        scale);
}

template<typename T>
winrt::ExpressionAnimation SnapPointWrapper<T>::CreateConditionalExpression(
    winrt::InteractionTracker const& interactionTracker,
    winrt::Compositor const& compositor,
    winrt::hstring const& target,
    winrt::hstring const& scale)
{
    winrt::SnapPointBase winrtSnapPoint = safe_cast<winrt::SnapPointBase>(m_snapPoint);
    SnapPointBase* snapPoint = winrt::get_self<SnapPointBase>(winrtSnapPoint);

    m_subExpressionsPropertySet = snapPoint->CreateSubExpressionsPropertySet(
        interactionTracker,
        compositor,
        target,
        &m_subExpression1,
        &m_subExpression2);

    return snapPoint->CreateConditionalExpression(
        m_subExpressionsPropertySet,
        m_actualApplicableZone,
        m_actualImpulseApplicableZone,
        compositor,
        target,
        scale);
}

template<typename T>
void SnapPointWrapper<T>::DetermineActualApplicableZone(
    SnapPointWrapper<T>* previousSnapPointWrapper,
    SnapPointWrapper<T>* nextSnapPointWrapper,
    bool forImpulse)
{
    winrt::SnapPointBase winrtSnapPoint = safe_cast<winrt::SnapPointBase>(m_snapPoint);
    SnapPointBase* snapPoint = winrt::get_self<SnapPointBase>(winrtSnapPoint);
    SnapPointBase* previousSnapPoint = nullptr;
    SnapPointBase* nextSnapPoint = nullptr;
    double previousIgnoredValue = NAN;
    double nextIgnoredValue = NAN;

    if (previousSnapPointWrapper)
    {
        winrt::SnapPointBase winrtPreviousSnapPoint = safe_cast<winrt::SnapPointBase>(previousSnapPointWrapper->SnapPoint());
        previousSnapPoint = winrt::get_self<SnapPointBase>(winrtPreviousSnapPoint);
        previousIgnoredValue = previousSnapPointWrapper->m_ignoredValue;
    }

    if (nextSnapPointWrapper)
    {
        winrt::SnapPointBase winrtNextSnapPoint = safe_cast<winrt::SnapPointBase>(nextSnapPointWrapper->SnapPoint());
        nextSnapPoint = winrt::get_self<SnapPointBase>(winrtNextSnapPoint);
        nextIgnoredValue = nextSnapPointWrapper->m_ignoredValue;
    }

    if (!forImpulse)
    {
        m_actualApplicableZone = snapPoint->DetermineActualApplicableZone(
            previousSnapPoint,
            nextSnapPoint);
    }

    m_actualImpulseApplicableZone = snapPoint->DetermineActualImpulseApplicableZone(
        previousSnapPoint,
        nextSnapPoint,
        m_ignoredValue,
        previousIgnoredValue,
        nextIgnoredValue);
}

template<typename T>
void SnapPointWrapper<T>::Combine(SnapPointWrapper<T>* snapPointWrapper)
{
    winrt::SnapPointBase winrtSnapPoint = safe_cast<winrt::SnapPointBase>(m_snapPoint);
    SnapPointBase* snapPoint = winrt::get_self<SnapPointBase>(winrtSnapPoint);

    snapPoint->Combine(m_combinationCount, snapPointWrapper->SnapPoint());
}

template<typename T>
double SnapPointWrapper<T>::Evaluate(double value) const
{
    winrt::SnapPointBase winrtSnapPoint = safe_cast<winrt::SnapPointBase>(m_snapPoint);
    SnapPointBase* snapPoint = winrt::get_self<SnapPointBase>(winrtSnapPoint);

    return snapPoint->Evaluate(m_actualApplicableZone, value);
}

template<typename T>
bool SnapPointWrapper<T>::SnapsAt(double value) const
{
    winrt::SnapPointBase winrtSnapPoint = safe_cast<winrt::SnapPointBase>(m_snapPoint);
    SnapPointBase* snapPoint = winrt::get_self<SnapPointBase>(winrtSnapPoint);

    return snapPoint->SnapsAt(m_actualApplicableZone, value);
}

template SnapPointWrapper<winrt::ScrollSnapPointBase>::SnapPointWrapper(winrt::ScrollSnapPointBase const& snapPoint);
template SnapPointWrapper<winrt::ZoomSnapPointBase>::SnapPointWrapper(winrt::ZoomSnapPointBase const& snapPoint);

#ifdef _DEBUG
template SnapPointWrapper<winrt::ScrollSnapPointBase>::~SnapPointWrapper();
template SnapPointWrapper<winrt::ZoomSnapPointBase>::~SnapPointWrapper();
#endif //_DEBUG

template winrt::ScrollSnapPointBase SnapPointWrapper<winrt::ScrollSnapPointBase>::SnapPoint() const;
template winrt::ZoomSnapPointBase SnapPointWrapper<winrt::ZoomSnapPointBase>::SnapPoint() const;

template std::tuple<double, double> SnapPointWrapper<winrt::ScrollSnapPointBase>::ActualApplicableZone() const;
template std::tuple<double, double> SnapPointWrapper<winrt::ZoomSnapPointBase>::ActualApplicableZone() const;

template int SnapPointWrapper<winrt::ScrollSnapPointBase>::CombinationCount() const;
template int SnapPointWrapper<winrt::ZoomSnapPointBase>::CombinationCount() const;

template winrt::ExpressionAnimation SnapPointWrapper<winrt::ScrollSnapPointBase>::CreateRestingPointExpression(
    winrt::Compositor const& compositor,
    winrt::hstring const& target,
    winrt::hstring const& scale);
template winrt::ExpressionAnimation SnapPointWrapper<winrt::ZoomSnapPointBase>::CreateRestingPointExpression(
    winrt::Compositor const& compositor,
    winrt::hstring const& target,
    winrt::hstring const& scale);

template winrt::ExpressionAnimation SnapPointWrapper<winrt::ScrollSnapPointBase>::CreateConditionalExpression(
    winrt::InteractionTracker const& interactionTracker,
    winrt::Compositor const& compositor,
    winrt::hstring const& target,
    winrt::hstring const& scale);
template winrt::ExpressionAnimation SnapPointWrapper<winrt::ZoomSnapPointBase>::CreateConditionalExpression(
    winrt::InteractionTracker const& interactionTracker,
    winrt::Compositor const& compositor,
    winrt::hstring const& target,
    winrt::hstring const& scale);

template void SnapPointWrapper<winrt::ScrollSnapPointBase>::DetermineActualApplicableZone(
    SnapPointWrapper<winrt::ScrollSnapPointBase>* previousSnapPoint,
    SnapPointWrapper<winrt::ScrollSnapPointBase>* nextSnapPoint,
    bool forImpulse);
template void SnapPointWrapper<winrt::ZoomSnapPointBase>::DetermineActualApplicableZone(
    SnapPointWrapper<winrt::ZoomSnapPointBase>* previousSnapPoint,
    SnapPointWrapper<winrt::ZoomSnapPointBase>* nextSnapPoint,
    bool forImpulse);

template void SnapPointWrapper<winrt::ScrollSnapPointBase>::Combine(SnapPointWrapper<winrt::ScrollSnapPointBase>* snapPointWrapper);
template void SnapPointWrapper<winrt::ZoomSnapPointBase>::Combine(SnapPointWrapper<winrt::ZoomSnapPointBase>* snapPointWrapper);

template double SnapPointWrapper<winrt::ScrollSnapPointBase>::Evaluate(double value) const;
template double SnapPointWrapper<winrt::ZoomSnapPointBase>::Evaluate(double value) const;

template bool SnapPointWrapper<winrt::ScrollSnapPointBase>::SnapsAt(double value) const;
template bool SnapPointWrapper<winrt::ZoomSnapPointBase>::SnapsAt(double value) const;

template bool SnapPointWrapper<winrt::ScrollSnapPointBase>::ResetIgnoredValue();
template bool SnapPointWrapper<winrt::ZoomSnapPointBase>::ResetIgnoredValue();

template void SnapPointWrapper<winrt::ScrollSnapPointBase>::SetIgnoredValue(double ignoredValue);
template void SnapPointWrapper<winrt::ZoomSnapPointBase>::SetIgnoredValue(double ignoredValue);
