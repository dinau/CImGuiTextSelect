#include <stddef.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef struct TextSelect TextSelect;

// ユーザー提供の関数ポインタ型
typedef const char* (*GetLineAtIdxFn)(size_t idx, void* userdata, size_t* out_len);
typedef size_t (*GetNumLinesFn)(void* userdata);

// コンストラクタ / デストラクタ
TextSelect* textselect_create(GetLineAtIdxFn getLineAtIdx,
                              GetNumLinesFn getNumLines,
                              void* userdata,
                              int enableWordWrap);

void textselect_destroy(TextSelect* ts);

// C++ public API のラッパー
int  textselect_has_selection(const TextSelect* ts);
void textselect_copy(const TextSelect* ts);
void textselect_select_all(TextSelect* ts);
void textselect_update(TextSelect* ts);
void textselect_clear_selection(TextSelect* ts);

#ifdef __cplusplus
}
#endif
