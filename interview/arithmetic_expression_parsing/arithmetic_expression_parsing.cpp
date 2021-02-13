#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <math.h>
using namespace std;

typedef enum {
  OP_ADD = 0,
  OP_SUB,
  OP_MUL,
  OP_DIV,
  OP_SIN,
  OP_COS,
  LEFT_PARENTHESIS,
  RIGHT_PARENTHESIS,
  NUM_FLOAT,
}ExpressionUnitType;

const char *str_ops[] = {"+", "-", "*", "/", "sin", "cos", "(", ")"};

typedef struct {
  ExpressionUnitType type;
  float value;
}ExpressionUnit;

bool IsUnaryOp(vector<ExpressionUnit> &v_expression, int32_t index) {
  if (index < 0) {
    return false;
  }
  if (index >= v_expression.size()) {
    return false;
  }
  ExpressionUnitType op_type = v_expression[index].type;
  return ((OP_SIN == op_type) || (OP_COS == op_type));
}

bool ReplaceExpressionWithResult(vector<ExpressionUnit> &v_expression,
                                 int32_t start_pos,
                                 int32_t end_pos,
                                 float result) {
  bool keep_par = false;
  if (LEFT_PARENTHESIS == v_expression[start_pos].type) {
    if (IsUnaryOp(v_expression, start_pos - 1)) {
      keep_par = true;
    }
  }
  int32_t set_value_pos, erase_start_pos, erase_end_pos;
  if (keep_par) {
    set_value_pos = start_pos + 1;
    erase_start_pos = start_pos + 2;
    erase_end_pos = end_pos - 1;
  } else {
    set_value_pos = start_pos;
    erase_start_pos = start_pos + 1;
    erase_end_pos = end_pos;
  }
  for (int i = erase_end_pos; i >= erase_start_pos ; --i) {
    v_expression.erase(v_expression.begin() + i);
  }
  v_expression[set_value_pos].type = NUM_FLOAT;
  v_expression[set_value_pos].value = result;
  return true;
}

bool RunInnerBinaryExpression(vector<ExpressionUnit> &v_expression,
                              int32_t inner_left_par_pos) {
  uint32_t pos_a = inner_left_par_pos + 1;
  uint32_t pos_bin_op = pos_a + 1;
  uint32_t pos_b = pos_bin_op + 1;
  if (NUM_FLOAT != v_expression[pos_a].type) {
    return false;
  }
  if (NUM_FLOAT != v_expression[pos_b].type) {
    return false;
  }
  if (v_expression[pos_bin_op].type > OP_DIV) {
    return false;
  }
  float result;
  float a = v_expression[pos_a].value;
  float b = v_expression[pos_b].value;
  switch (v_expression[pos_bin_op].type) {
    case OP_ADD:
      result = a + b;
      break;
    case OP_SUB:
      result = a - b;
      break;
    case OP_MUL:
      result = a * b;
      break;
    case OP_DIV:
      result = a / b;
      break;
  }

  ReplaceExpressionWithResult(v_expression,
                              inner_left_par_pos,
                              inner_left_par_pos + 4,
                              result);
  return true;
}

bool RunInnerUnaryExpression(vector<ExpressionUnit> &v_expression,
                             int32_t inner_left_par_pos) {
  uint32_t pos_a = inner_left_par_pos + 1;
  uint32_t pos_op = inner_left_par_pos - 1;
  bool valid_unary_op = false;
  if (pos_op >= 0) {
    if ((OP_SIN == v_expression[pos_op].type) || (OP_COS == v_expression[pos_op].type)) {
      valid_unary_op = true;
    }
  }
  if (NUM_FLOAT != v_expression[pos_a].type) {
    return false;
  }
  float result;
  if (pos_op >= 0) {
    switch (v_expression[pos_op].type) {
      case OP_SIN:
        result = sin(v_expression[pos_a].value);
        break;
      case OP_COS:
        result = cos(v_expression[pos_a].value);
        break;
      defalut:
        valid_unary_op = false;
    }
  } else {
    valid_unary_op = false;
  }
  if (valid_unary_op) {
    ReplaceExpressionWithResult(v_expression,
                                inner_left_par_pos - 1,
                                inner_left_par_pos + 2,
                                result);
  } else {
    v_expression.erase(v_expression.begin() + inner_left_par_pos + 2);
    v_expression.erase(v_expression.begin() + inner_left_par_pos);
  }
  return true;
}

void ShowVec(const vector<ExpressionUnit> &vec) {
  printf("--------------------- ShowVec[%lu] -------------------\n", vec.size());
  for(uint32_t i = 0; i < vec.size(); i++) {
    if (NUM_FLOAT == vec[i].type) {
      printf("%f\n", vec[i].value);
    } else {
      printf("%s\n", str_ops[vec[i].type]);
    }
  }
}

bool StrToExpressionVector(const char * input_string, vector<ExpressionUnit> &v_expression) {
  ExpressionUnit expression_unit;
  const char *p_temp = input_string;
  char *p_end;
  bool op_fit;
  while (0 != (*p_temp)) {
    if (' ' == *p_temp) {
      p_temp++;
    } else {
      op_fit = false;
      for (uint32_t i = 0; i < sizeof(str_ops) / sizeof(const char *); i++) {
        if (0 == strncasecmp(str_ops[i], p_temp, strlen(str_ops[i]))) {
          expression_unit.type = static_cast<ExpressionUnitType>(i);
          v_expression.push_back(expression_unit);
          p_temp += strlen(str_ops[i]);
          op_fit = true;
          break;
        }
      }
      if (!op_fit) {
        expression_unit.type = NUM_FLOAT;
        expression_unit.value = strtof(p_temp, &p_end);
        if (p_end == p_temp) {
          printf("Expression error\n");
          return false;
        }
        v_expression.push_back(expression_unit);
        p_temp = p_end;
      }
    }
  }
  return true;
}

float ExpressionParsing(const char * input_string) {
  vector<ExpressionUnit> v_expression;
  if (!StrToExpressionVector(input_string, v_expression)) {
    return 0.0;
  }

  while (v_expression.size() > 1) {
    int32_t inner_left_par_pos = -1, inner_right_par_pos = -1;
    for(uint32_t i = 0; i < v_expression.size(); i++) {
      if (LEFT_PARENTHESIS == v_expression[i].type) {
        inner_left_par_pos = i;
      } else if (RIGHT_PARENTHESIS == v_expression[i].type) {
        inner_right_par_pos = i;
        if (inner_left_par_pos < 0) {
          return false;
        }
        uint32_t unit_num = inner_right_par_pos - inner_left_par_pos - 1;
        if (3 == unit_num) {
          RunInnerBinaryExpression(v_expression, inner_left_par_pos);
          break;
        } else if (1 == unit_num) {
          RunInnerUnaryExpression(v_expression, inner_left_par_pos);
          break;
        }
      }
    }
  }
  return v_expression[0].value;
}




int main() {
  const char * expression_string[] = {"((1+sin(0))*(3.0+(4*5)))",
                                      "((1+sin(2.0+1))*((3.0+(4*(5/3.0)))-11))"};
  for (int i = 0; i < sizeof(expression_string) / sizeof(const char *); ++i) {
    printf("%s: %f\n", expression_string[i], ExpressionParsing(expression_string[i]));
  }
  return 0;
}
