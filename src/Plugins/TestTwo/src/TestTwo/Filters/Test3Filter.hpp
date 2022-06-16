#pragma once

#include "complex/Filter/FilterTraits.hpp"
#include "complex/Filter/IFilter.hpp"

#include "TestTwo/TestTwo_export.hpp"
namespace complex
{
class TESTTWO_EXPORT Test3Filter : public complex::IFilter
{
public:
  Test3Filter();
  ~Test3Filter() override;

  // Parameter Keys
  static inline constexpr StringLiteral k_OperatorType_Key = "OperatorType";
  static inline constexpr StringLiteral k_NumParameter_Key = "NumParameter";
  static inline constexpr StringLiteral k_InputArrayPath_Key = "InputArrayPath";
  static inline constexpr StringLiteral k_OutputArray_Key = "OutputArray";
  static inline constexpr StringLiteral k_NumericType_Key = "numericType";

  enum class OperatorType : uint64
  {
    Plus = 0,
    Minus = 1,
    Multiply = 2,
    Divide = 3
  };

  /**
   * @brief Returns the name of the filter.
   * @return std::string
   */
  std::string name() const override;

  /**
   * @brief Returns the C++ classname of this filter.
   * @return
   */
  std::string className() const override;

  /**
   * @brief Returns the filters ID as a std::string.
   * @return complex::Uuid
   */
  complex::Uuid uuid() const override;

  /**
   * @brief Returns the filter's human label.
   * @return std::string
   */
  std::string humanName() const override;

  /**
   * @brief Returns a collection of parameters used.
   * @return Parameters
   */
  complex::Parameters parameters() const override;

  /**
   * @brief Returns a unique_pointer to a copy of the filter.
   * @return complex::IFilter::UniquePointer
   */
  UniquePointer clone() const override;

protected:
  /**
   * @brief Filter-specifics for performing dataCheck.
   * @param data
   * @param args
   * @param messageHandler
   * @return Result<OutputActions>
   */
  PreflightResult preflightImpl(const complex::DataStructure& data, const complex::Arguments& args, const MessageHandler& messageHandler, const std::atomic_bool& shouldCancel) const override;

  /**
   * @brief Filter-specifics for performing execute.
   * @param data
   * @param args
   * @param pipelineNode
   * @param messageHandler
   * @return Result<>
   */
  complex::Result<> executeImpl(complex::DataStructure& data, const complex::Arguments& args, const PipelineFilter* pipelineNode, const MessageHandler& messageHandler,
                                const std::atomic_bool& shouldCancel) const override;
};
} // namespace complex
COMPLEX_DEF_FILTER_TRAITS(complex, Test3Filter, "ad9cf22b-bc5e-41d6-b02e-ef49ffd12c04");
