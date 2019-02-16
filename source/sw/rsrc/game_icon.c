
#include "sdl_inc.h"
#include "sdlappicon.h"

static Uint8 sdlappicon_pixels[] = {
  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0l\342\32"
  "\32w\346Zx\251\343\200\275\321\346\253\316\332\346\327\323\334\347\370\346"
  "\346\347\377\347\347\347\377\347\347\347\367\350\350\350\344\351\351\351"
  "\306\267\333\351\242\235\325\354x\353\353\353'\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0\0\0\0q\354\33\0n\350y\21t\343\267s\246\340\310\342\344\347\365\350\350"
  "\350\377\351\351\351\377\351\351\351\377\351\351\351\377\351\351\351\377"
  "\351\351\351\377\303\336\351\377\331\344\351\377\351\351\351\377\210\314"
  "\350\377\350\350\350\377\324\342\347\377\314\337\347\325\346\346\346o\341"
  "\341\341\21\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0\0l\350!\2n\350\234\1m\347\367G\220\342\310\344\346\350\371\351\351\351"
  "\377\352\352\352\377\352\352\352\377\353\353\353\377\353\353\353\377\353"
  "\353\353\377\353\353\353\377\353\353\353\377\353\353\353\377\353\353\353"
  "\377\267\334\353\377\16\251\352\377\230\322\352\377.\261\351\377\204\314"
  "\351\377\350\350\350\377\347\347\347\366\317\340\350\236`\312\3640\0\0\0"
  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0`\337\10\0k\345\201\1m\347\366\1m\347\367\212"
  "\263\344\315\352\352\352\377\353\353\353\377\353\353\353\377\354\354\354"
  "\377\354\354\354\377\355\355\355\377\355\355\355\377\355\355\355\377\355"
  "\355\355\377\355\355\355\377\355\355\355\377:\270\355\377\17\253\355\377"
  "\1\247\354\377\1\247\354\377\1\246\353\377\7\250\353\377:\266\352\377\215"
  "\316\351\377\307\336\350\377\325\342\347\363\345\345\345\201\350\350\350"
  "\13\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0\0\0\0\0\0q\350-\1l\347\325\1m\350\377\1n\351\377z\254\344\310\353\353"
  "\353\377\354\354\354\377\354\354\354\377\355\355\355\377\356\356\356\377"
  "\356\356\356\377\357\357\357\377\357\357\357\377\302\342\357\377\275\340"
  "\357\377\346\354\357\377\201\317\357\377\2\251\357\377\1\251\357\377\1\250"
  "\356\377\1\250\356\377\1\247\355\377\1\247\354\377\1\247\354\377J\273\353"
  "\377\350\351\352\377\351\351\351\377\350\350\350\377\351\351\351\336\352"
  "\352\3521\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0n\347_\1m\347\367\1m\350\377\1n\352\377\36|\346\312\354\354\354\377\355"
  "\355\355\377\356\356\356\377\356\356\356\377\357\357\357\377\337\353\360"
  "\3779\272\360\377g\310\361\377\326\350\361\377\351\357\361\377^\306\361\377"
  "\7\254\361\377\1\252\361\377\1\252\361\377\1\252\361\377\1\251\360\377\1"
  "\251\360\377\1\251\357\377\1\250\356\377\1\250\356\377\1\247\355\377G\273"
  "\354\377\274\335\353\377\352\352\352\377\350\350\350\377\347\347\347\363"
  "\346\346\346Z\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0o\350z\1m\347"
  "\376\1n\351\377\1n\352\377\1o\353\377\272\320\351\337\355\355\355\377\357"
  "\357\357\377\357\357\357\377\360\360\360\377\361\361\361\377\362\362\362"
  "\377\347\356\362\377\325\352\363\3778\274\363\377`\307\363\377t\316\364\377"
  "\1\254\364\377\1\254\363\377\27\262\363\377r\315\363\377\247\334\362\377"
  "\206\322\362\377!\264\361\377\1\251\360\377\1\251\357\377;\272\357\377\235"
  "\326\355\377\246\330\354\377\223\321\353\377\265\332\352\377\351\351\351"
  "\377\347\347\347\376\351\351\351\202\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0m\351"
  "u\1m\347\377\1n\351\377\1n\352\377\1o\354\377\21w\351\325\356\356\356\377"
  "\357\357\357\377\360\360\360\377\361\361\361\377\362\362\362\377\363\363"
  "\363\377\364\364\364\377\364\364\364\377\365\365\365\377\361\364\365\377"
  "\276\346\366\377'\270\366\377\1\256\366\377\40\266\366\377\342\357\365\377"
  "\365\365\365\377\364\364\364\377\364\364\364\377\345\357\363\3773\272\362"
  "\377\1\252\361\377\313\346\360\377\356\357\357\377\326\347\356\377\355\355"
  "\355\377\354\354\354\377\352\352\352\377\351\351\351\377\347\347\347\377"
  "\352\352\352x\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0m\346[\1m\347\375\1n\351\377\1n\352\377\1o"
  "\354\377\1p\355\377P\230\352\303\360\360\360\377\361\361\361\377\362\362"
  "\362\377\363\363\363\377\364\364\364\377\365\365\365\377\366\366\366\377"
  "\366\366\366\377\367\367\367\377\367\367\367\377\370\370\370\377S\307\370"
  "\377\1\257\370\377\227\334\370\377\367\367\367\377\367\367\367\377\366\366"
  "\366\377\366\366\366\377\365\365\365\377\315\351\364\377\2\254\363\3774\272"
  "\362\377`\306\361\377\307\344\360\377\357\357\357\377\355\355\355\377\354"
  "\354\354\377\352\352\352\377\351\351\351\377\347\347\347\375\350\350\350"
  "Y\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0p\3440\1m\347\365\1n\351\377\1n\352\377\1o\354\377\1p\355\377\1p\357\377"
  "\201\263\354\307\361\361\361\377\363\363\363\377\364\364\364\377\365\365"
  "\365\377\366\366\366\377\367\367\367\377\370\370\370\377\364\367\370\377"
  "t\322\371\377\212\330\371\377U\311\372\377\1\260\372\377\1\260\372\377\300"
  "\351\372\377\371\371\371\377\371\371\371\377\370\370\370\377\370\370\370"
  "\377\367\367\367\377\366\366\366\377\32\265\365\377\13\257\364\377\331\353"
  "\363\377\263\337\361\377\360\360\360\377\357\357\357\377\355\355\355\377"
  "\354\354\354\377\352\352\352\377\351\351\351\377\347\347\347\367\344\344"
  "\3440\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0`\337\10\1"
  "m\350\332\1m\350\377\1n\352\377\1o\354\377\1p\355\377\1p\357\377\1q\360\377"
  "\221\275\357\312\363\363\363\377\364\364\364\377\366\366\366\377\367\367"
  "\367\377\370\370\370\377\371\371\371\377\372\372\372\377\372\372\372\377"
  "\373\373\373\377\373\373\373\377\364\372\374\377\36\273\374\377\1\262\374"
  "\377\237\340\374\377\373\373\373\377\373\373\373\377\372\372\372\377\372"
  "\372\372\377\371\371\371\377\362\366\370\377\7\260\367\377;\277\366\377\364"
  "\364\364\377\363\363\363\377\362\362\362\377\360\360\360\377\357\357\357"
  "\377\355\355\355\377\354\354\354\377\352\352\352\377\350\350\350\377\347"
  "\347\347\326\377\377\377\10\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0\0m\350\203\1m\350\377\1n\351\377\1o\353\377\1p\355\377\1p\357\377\1q\360"
  "\377\1r\362\377\202\266\360\316\365\365\365\377\366\366\366\377\367\367\367"
  "\377\371\371\371\377\372\372\372\377\373\373\373\377\374\374\374\377\374"
  "\374\374\377\375\375\375\377\362\372\375\377\376\376\376\377m\323\376\377"
  "\1\263\376\377*\277\376\377\361\371\375\377\375\375\375\377\374\374\374\377"
  "\374\374\374\377\373\373\373\377\230\336\372\377\1\260\371\377\202\324\367"
  "\377\366\366\366\377\365\365\365\377\363\363\363\377\362\362\362\377\360"
  "\360\360\377\357\357\357\377\355\355\355\377\353\353\353\377\351\351\351"
  "\377\350\350\350\377\350\350\350\205\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0\0\0p\347\40\1m\347\367\1n\351\377\1o\353\377\1o\354\377\1p\356\377\1q"
  "\360\377\1q\361\377\1r\363\377`\245\363\332\366\366\366\377\370\370\370\377"
  "\371\371\371\377\372\372\372\377\373\373\373\377\374\374\374\377\375\375"
  "\375\377\277\353\376\377\377\377\377\377\346\370\377\377\377\377\377\377"
  "\233\342\377\377\1\264\377\377\1\264\377\3778\304\377\377\303\355\377\377"
  "\355\371\376\377\336\364\375\377\204\330\374\377\10\263\373\377\1\260\372"
  "\3770\275\371\377\370\370\370\377\366\366\366\377\365\365\365\377\363\363"
  "\363\377\361\361\361\377\352\356\360\377\275\340\356\377\354\354\354\377"
  "\353\353\353\377\351\351\351\377\347\347\347\364\352\352\352\30\0\0\0\0\0"
  "\0\0\0\0\0\0\0\0\0\0\0\2m\347\237\1m\350\377\1n\352\377\1o\354\377\1p\355"
  "\377\1p\357\377\1q\361\377\1r\363\377\1s\364\377)\210\365\356\370\370\370"
  "\377\371\371\371\377\373\373\373\377\374\374\374\377\375\375\375\377\376"
  "\376\376\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\346\370\377\377\21\271\377\377\1\264\377\377\1\264\377"
  "\377\1\264\377\377\1\264\377\377\1\264\377\377\1\263\376\377\1\263\375\377"
  "\1\262\374\377\2\261\373\377\331\357\371\377\370\370\370\377\366\366\366"
  "\377\364\364\364\377\363\363\363\377\361\361\361\377\357\357\357\377\355"
  "\355\355\377\354\354\354\377\352\352\352\377\350\350\350\377\347\347\347"
  "\224\0\0\0\0\0\0\0\0\0\0\0\0\0j\352\30\1l\350\372\1n\351\377\1o\353\377\1"
  "p\355\377\1p\357\377\1q\360\377\1r\362\377\1s\364\377\1t\366\377\2u\367\377"
  "\333\351\371\377\373\373\373\377\374\374\374\377\375\375\375\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\276\354\377\377\1\264\377"
  "\377\1\264\377\377\1\264\377\377\1\264\377\377\1\264\377\377\1\264\377\377"
  "\1\264\377\377\1\263\375\377\1\262\374\377\205\331\373\377\371\371\371\377"
  "\367\367\367\377\366\366\366\377\364\364\364\377\362\362\362\377\360\360"
  "\360\377\357\357\357\377\355\355\355\377\353\353\353\377\351\351\351\377"
  "\347\347\347\367\350\350\350\26\0\0\0\0\0\0\0\0\0n\350y\1m\350\377\1n\352"
  "\377\1o\354\377\1p\356\377\1p\357\377\1q\361\377\1r\363\377\1s\365\377\1"
  "t\367\377\1u\371\377r\261\372\377\374\374\374\377\375\375\375\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\307\357"
  "\377\377~\331\377\377\244\344\377\377\1\264\377\377\1\264\377\377\1\264\377"
  "\377\1\264\377\377\1\264\377\377\1\264\377\377\1\263\375\377\27\270\374\377"
  "\352\365\372\377\371\371\371\377\367\367\367\377\365\365\365\377\363\363"
  "\363\377\361\361\361\377\357\357\357\377\356\356\356\377\354\354\354\377"
  "\352\352\352\377\350\350\350\377\350\350\350|\0\0\0\0\0\0\0\0\1m\350\320"
  "\1n\351\377\1o\353\377\1o\354\377\1p\356\377\1q\360\377\1r\362\377\1s\364"
  "\377\1t\366\377\1u\370\377\1v\372\377\12{\373\377\336\355\375\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377l\323\377\377\353\371\377\377\377\377\377\377<\305\377\377\1\264"
  "\377\377\1\264\377\377\1\264\377\377\1\264\377\377\1\264\377\377\25\272\377"
  "\377_\316\375\377f\317\373\377\372\372\372\377\370\370\370\377\366\366\366"
  "\377\364\364\364\377\362\362\362\377\360\360\360\377\356\356\356\377\354"
  "\354\354\377\353\353\353\377\351\351\351\377\350\350\350\322\0\0\0\0\0k\346"
  "\37\1m\347\376\1n\351\377\1o\353\377\1p\355\377\1p\357\377\1q\361\377\1r"
  "\363\377\1s\365\377\1t\367\377\1u\371\377\1v\373\377\1w\374\377H\235\376"
  "\377\375\376\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377V\315\377\377\373\376\377\377\377\377\377\377\201\332"
  "\377\377\1\264\377\377\1\264\377\377\1\264\377\377\1\264\377\377\1\264\377"
  "\377\227\340\377\377\372\375\376\377T\313\374\377\372\373\373\377\371\371"
  "\371\377\367\367\367\377\365\365\365\377\363\363\363\377\361\361\361\377"
  "\357\357\357\377\355\355\355\377\353\353\353\377\351\351\351\377\347\347"
  "\347\376\347\347\347\40\0l\347a\1m\350\377\1n\352\377\1o\354\377\1p\356\377"
  "\1q\360\377\1r\362\377\1s\364\377\1t\366\377\1u\370\377\1v\372\377\1w\374"
  "\377\1w\375\377\1x\377\377\177\273\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\241\343\377\377\1\264\377\377\33\274\377\377\301\355\377"
  "\377\3\265\377\377\7\266\377\377\363\373\377\377\377\377\377\377\375\375"
  "\375\377\374\374\374\377\372\372\372\377\370\370\370\377\366\366\366\377"
  "\364\364\364\377\362\362\362\377\360\360\360\377\356\356\356\377\354\354"
  "\354\377\352\352\352\377\350\350\350\377\350\350\350d\2m\350\230\1m\350\377"
  "\1n\352\377\1o\354\377\1p\356\377\1q\360\377\1r\362\377\1s\364\377\1t\366"
  "\377\1u\370\377\1v\372\377\1w\374\377\1x\376\377\1x\377\377\1x\377\377\222"
  "\305\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\255\347\377\377\377\377\377\377\235\342\377\377\1\264\377\377"
  "\261\350\377\377\377\377\377\377\40\275\377\377F\310\377\377\377\377\377"
  "\377\377\377\377\377\376\376\376\377\374\374\374\377\372\372\372\377\370"
  "\370\370\377\366\366\366\377\364\364\364\377\362\362\362\377\360\360\360"
  "\377\356\356\356\377\354\354\354\377\352\352\352\377\350\350\350\377\350"
  "\350\350\230\1m\347\300\1n\351\377\1o\353\377\1p\355\377\1p\357\377\1q\361"
  "\377\1r\363\377\1s\365\377\1t\367\377\1u\371\377\1v\373\377\1w\375\377\1"
  "x\377\377\1x\377\377\1x\377\377\2y\377\377\236\314\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\337\366\377\377\377\377\377\377"
  "u\326\377\377\32\273\377\377\374\376\377\377\377\377\377\377*\300\377\377"
  "\222\337\377\377\377\377\377\377\377\377\377\377\377\377\377\377\375\375"
  "\375\377\373\373\373\377\371\371\371\377\367\367\367\377\365\365\365\377"
  "\363\363\363\377\361\361\361\377\357\357\357\377\355\355\355\377\353\353"
  "\353\377\351\351\351\377\350\350\350\300\1l\347\340\1n\351\377\1o\353\377"
  "\1p\355\377\1p\357\377\1q\361\377\1r\363\377\1s\365\377\1t\367\377\1u\371"
  "\377\1v\373\377\1w\375\377\1x\377\377\1x\377\377\1x\377\377\1x\377\377\2"
  "y\377\377o\262\377\377\370\373\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377?\306\377\377e\321\377\377\377\377\377\377\377\377\377"
  "\377\35\274\377\377\302\355\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\375\375\375\377\373\373\373\377\371\371\371\377\367\367\367\377"
  "\365\365\365\377\363\363\363\377\361\361\361\377\357\357\357\377\355\355"
  "\355\377\353\353\353\377\351\351\351\377\347\347\347\337\1m\350\366\1n\351"
  "\377\1o\353\377\1p\355\377\1p\357\377\1q\361\377\1r\363\377\1s\365\377\1"
  "u\370\377\1v\372\377\1w\374\377\1x\376\377\1x\377\377\1x\377\377\1x\377\377"
  "\1x\377\377\1x\377\377\1x\377\3772\222\377\377\325\351\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\376\377\377\377\14\267\377\377\207\334\377\377\377\377\377\377"
  "\377\377\377\377=\306\377\377\322\362\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\376\376\376\377\374\374\374\377\372\372\372\377\370"
  "\370\370\377\365\365\365\377\363\363\363\377\361\361\361\377\357\357\357"
  "\377\355\355\355\377\353\353\353\377\351\351\351\377\350\350\350\366\1m\347"
  "\377\1n\351\377\1o\353\377\1p\355\377\1p\357\377\1q\361\377\1s\364\377\1"
  "t\366\377\1u\370\377\1v\372\377\1w\374\377\1x\376\377\1x\377\377\1x\377\377"
  "\1x\377\377\1x\377\377\1x\377\377\1x\377\377\1x\377\377\7{\377\377y\270\377"
  "\377\363\371\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\365\374\377\377\1\264\377\377}\330\377\377\377\377\377\377\377"
  "\377\377\377l\323\377\377\250\345\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\376\376\376\377\374\374\374\377\372\372\372\377\370\370"
  "\370\377\366\366\366\377\364\364\364\377\361\361\361\377\357\357\357\377"
  "\355\355\355\377\353\353\353\377\351\351\351\377\350\350\350\377\1m\350\377"
  "\1n\351\377\1o\353\377\1p\355\377\1p\357\377\1q\361\377\1s\364\377\1t\366"
  "\377\1u\370\377\1v\372\377\1w\374\377\1x\376\377\1x\377\377\1x\377\377\1"
  "x\377\377\1x\377\377\1x\377\377\1x\377\377\1x\377\377\1x\377\377\1x\377\377"
  "\24\202\377\377\201\274\377\377\353\364\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\22\271\377\377g\322\377\377\377\377\377\377\377\377"
  "\377\377w\327\377\377\250\345\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\376\376\376\377\374\374\374\377\372\372\372\377\370\370\370"
  "\377\366\366\366\377\364\364\364\377\361\361\361\377\357\357\357\377\355"
  "\355\355\377\353\353\353\377\351\351\351\377\350\350\350\377\1m\350\365\1"
  "n\351\377\1o\353\377\1p\355\377\1p\357\377\1q\361\377\1r\363\377\1s\365\377"
  "\1u\370\377\1v\372\377\1w\374\377\1x\376\377\1x\377\377\1x\377\377\1x\377"
  "\377\1x\377\377\1x\377\377\1x\377\377\1x\377\377\1x\377\377\1x\377\377\1"
  "x\377\377\1x\377\377\10|\377\377\\\250\377\377\320\346\377\377\377\377\377"
  "\377.\301\377\377X\316\377\377\377\377\377\377\377\377\377\377\225\340\377"
  "\377d\321\377\377\377\377\377\377\377\377\377\377\377\377\377\377\376\376"
  "\376\377\374\374\374\377\372\372\372\377\370\370\370\377\365\365\365\377"
  "\363\363\363\377\361\361\361\377\357\357\357\377\355\355\355\377\353\353"
  "\353\377\351\351\351\377\350\350\350\365\1l\347\341\1n\351\377\1o\353\377"
  "\1p\355\377\1p\357\377\1q\361\377\1r\363\377\1s\365\377\1t\367\377\1u\371"
  "\377\1v\373\377\1w\375\377\1x\377\377\1x\377\377\1x\377\377\1x\377\377\1"
  "x\377\377\1x\377\377\1x\377\377\1x\377\377\1x\377\377\1x\377\377\1x\377\377"
  "\1x\377\377\1x\377\377\2y\377\377U\244\377\377B\305\377\377Y\316\377\377"
  "\377\377\377\377\377\377\377\377\264\351\377\377\203\333\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\375\375\375\377\373\373\373\377"
  "\371\371\371\377\367\367\367\377\365\365\365\377\363\363\363\377\361\361"
  "\361\377\357\357\357\377\355\355\355\377\353\353\353\377\351\351\351\377"
  "\347\347\347\340\1m\347\302\1n\351\377\1o\353\377\1p\355\377\1p\357\377\1"
  "q\361\377\1r\363\377\1s\365\377\1t\367\377\1u\371\377\1v\373\377\1w\375\377"
  "\1x\377\377\1x\377\377\1x\377\377\1x\377\377\1x\377\377\1x\377\377\1x\377"
  "\377\1x\377\377\1x\377\377\1x\377\377\1x\377\377\1x\377\377\1x\377\377\1"
  "x\377\377\1x\377\377\5\240\377\377<\271\377\377\373\375\377\377\377\377\377"
  "\377\221\337\377\377\277\354\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\375\375\375\377\373\373\373\377\371\371\371\377\367\367\367"
  "\377\365\365\365\377\363\363\363\377\361\361\361\377\357\357\357\377\355"
  "\355\355\377\353\353\353\377\351\351\351\377\347\347\347\274\2l\346\216\1"
  "m\350\377\1n\352\377\1o\354\377\1p\356\377\1q\360\377\1r\362\377\1s\364\377"
  "\1t\366\377\1u\370\377\1v\372\377\1w\374\377\1x\376\377\1x\377\377\1x\377"
  "\377\1x\377\377\1x\377\377\1x\377\377\1x\377\377\1x\377\377\1x\377\377\1"
  "x\377\377\1x\377\377\1x\377\377\1x\377\377\1x\377\377\1x\377\377\1\232\377"
  "\377\1\222\377\377;\227\377\377\346\362\377\377T\314\377\377\336\365\377"
  "\377\377\377\377\377\377\377\377\377\376\376\376\377\374\374\374\377\372"
  "\372\372\377\370\370\370\377\366\366\366\377\364\364\364\377\362\362\362"
  "\377\360\360\360\377\356\356\356\377\354\354\354\377\352\352\352\377\350"
  "\350\350\377\347\347\347\223\0o\346Z\1m\350\377\1n\352\377\1o\354\377\1p"
  "\356\377\1q\360\377\1r\362\377\1s\364\377\1t\366\377\1u\370\377\1v\372\377"
  "\1w\374\377\1w\375\377\1x\377\377\1x\377\377\1x\377\377\1x\377\377\1x\377"
  "\377\1x\377\377\1x\377\377\1x\377\377\1x\377\377\1x\377\377\1x\377\377\1"
  "x\377\377\1x\377\377\1x\377\377\1\231\377\377\1\217\377\377\1x\377\377\40"
  "\210\377\377\242\333\377\377\374\376\377\377\377\377\377\377\377\377\377"
  "\377\375\375\375\377\374\374\374\377\372\372\372\377\370\370\370\377\366"
  "\366\366\377\364\364\364\377\362\362\362\377\360\360\360\377\356\356\356"
  "\377\354\354\354\377\352\352\352\377\350\350\350\377\352\352\352_\0r\355"
  "\35\1m\347\376\1n\351\377\1o\353\377\1p\355\377\1p\357\377\1q\361\377\1r"
  "\363\377\1s\365\377\1t\367\377\1u\371\377\1v\373\377\1w\374\377\1x\376\377"
  "\1x\377\377\1x\377\377\1x\377\377\1x\377\377\1x\377\377\1x\377\377\1x\377"
  "\377\1x\377\377\1x\377\377\1x\377\377\1x\377\377\1x\377\377\1x\377\377\1"
  "\230\377\377\1\216\377\377\1x\377\377\1x\377\377\21\200\377\377\317\346\377"
  "\377\377\377\377\377\376\376\376\377\374\374\374\377\373\373\373\377\371"
  "\371\371\377\367\367\367\377\365\365\365\377\363\363\363\377\361\361\361"
  "\377\357\357\357\377\355\355\355\377\353\353\353\377\351\351\351\377\347"
  "\347\347\375\350\350\350\26\0\0\0\0\1l\347\306\1n\351\377\1o\353\377\1o\354"
  "\377\1p\356\377\1q\360\377\1r\362\377\1s\364\377\1t\366\377\1u\370\377\1"
  "v\372\377\1v\373\377\1w\375\377\1x\377\377\1x\377\377\1x\377\377\1x\377\377"
  "\1x\377\377\1x\377\377\1x\377\377\1x\377\377\1x\377\377\1x\377\377\1x\377"
  "\377\1x\377\377\1x\377\377\1\232\377\377\1\214\377\377\1x\377\377\1x\377"
  "\377\1x\377\377\35\207\377\377\351\363\377\377\375\375\375\377\373\373\373"
  "\377\372\372\372\377\370\370\370\377\366\366\366\377\364\364\364\377\362"
  "\362\362\377\360\360\360\377\356\356\356\377\354\354\354\377\353\353\353"
  "\377\351\351\351\377\351\351\351\317\0\0\0\0\0\0\0\0\0n\347k\1m\350\377\1"
  "n\352\377\1o\354\377\1p\356\377\1p\357\377\1q\361\377\1r\363\377\1s\365\377"
  "\1t\367\377\1u\370\377\1v\372\377\1w\374\377\1w\375\377\1x\377\377\1x\377"
  "\377\1x\377\377\1x\377\377\1x\377\377\1x\377\377\1x\377\377\1x\377\377\1"
  "x\377\377\1x\377\377\1x\377\377\1x\377\377\1\232\377\377\1\212\377\377\1"
  "x\377\377\1x\377\377\1x\377\377\1x\377\377M\240\375\377\374\374\374\377\372"
  "\372\372\377\370\370\370\377\367\367\367\377\365\365\365\377\363\363\363"
  "\377\361\361\361\377\357\357\357\377\356\356\356\377\354\354\354\377\352"
  "\352\352\377\350\350\350\377\351\351\351r\0\0\0\0\0\0\0\0\0k\344\23\1m\347"
  "\365\1n\351\377\1o\353\377\1p\355\377\1p\357\377\1q\360\377\1r\362\377\1"
  "s\364\377\1t\366\377\1t\367\377\1u\371\377\1v\372\377\1w\374\377\1w\375\377"
  "\1x\377\377\1x\377\377\1x\377\377\1x\377\377\1x\377\377\1x\377\377\1x\377"
  "\377\1x\377\377\1x\377\377\1x\377\377\1x\377\377\1\235\377\377\1\212\377"
  "\377\1x\377\377\1x\377\377\1x\377\377\1w\375\377\1w\374\377\257\322\372\377"
  "\371\371\371\377\367\367\367\377\366\366\366\377\364\364\364\377\362\362"
  "\362\377\360\360\360\377\357\357\357\377\355\355\355\377\353\353\353\377"
  "\351\351\351\377\347\347\347\366\350\350\350\26\0\0\0\0\0\0\0\0\0\0\0\0\2"
  "l\346\216\1m\350\377\1n\352\377\1o\354\377\1p\355\377\1p\357\377\1q\361\377"
  "\1r\363\377\1s\364\377\1t\366\377\1u\370\377\1u\371\377\1v\372\377\1w\374"
  "\377\1w\375\377\1x\376\377\1x\377\377\1x\377\377\1x\377\377\1x\377\377\1"
  "x\377\377\1x\377\377\1x\377\377\1x\377\377\1x\377\377\1\233\377\377\1\215"
  "\377\377\1x\377\377\1x\376\377\1w\375\377\1w\374\377\1v\372\3774\221\371"
  "\377\370\370\370\377\366\366\366\377\364\364\364\377\363\363\363\377\361"
  "\361\361\377\357\357\357\377\355\355\355\377\354\354\354\377\352\352\352"
  "\377\350\350\350\377\351\351\351\226\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0a\333"
  "\25\1m\347\361\1n\351\377\1o\353\377\1o\354\377\1p\356\377\1q\360\377\1q"
  "\361\377\1r\363\377\1s\365\377\1t\366\377\1u\370\377\1u\371\377\1v\372\377"
  "\1v\373\377\4y\374\377N\240\375\377\203\275\376\377l\261\377\377\32\205\377"
  "\377\1x\377\377\1x\377\377\1x\377\377\1x\377\377\1x\377\377\1\225\377\377"
  "\1\221\376\377\1w\375\377\1w\374\377\1v\373\377\1v\372\377\1u\371\377\1u"
  "\370\377\314\340\366\377\365\365\365\377\363\363\363\377\361\361\361\377"
  "\360\360\360\377\356\356\356\377\354\354\354\377\353\353\353\377\351\351"
  "\351\377\347\347\347\361\351\351\351\27\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0\0\0\0n\351\200\1m\350\377\1n\351\377\1o\353\377\1p\355\377\1p\357\377"
  "\1q\360\377\1r\362\377\1r\363\377\1s\365\377\1t\366\377\1t\367\377\1u\370"
  "\377\15|\372\377\303\335\373\377\374\374\374\377\374\374\374\377\375\375"
  "\375\377\360\366\375\377N\240\376\377\1x\376\377\1x\376\377\1x\376\377\1"
  "w\375\377\1\211\375\377\1\231\374\377\1w\374\377\1v\373\377\1v\372\377\1"
  "u\370\377\1t\367\377\1t\366\377\207\273\365\377\363\363\363\377\362\362\362"
  "\377\360\360\360\377\357\357\357\377\355\355\355\377\353\353\353\377\351"
  "\351\351\377\350\350\350\377\351\351\351\177\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0\0\0\0\0\0\0\0\0\0\0f\314\5\1m\346\312\1m\350\377\1n\352\377\1o\354\377"
  "\1p\355\377\1p\357\377\1q\360\377\1r\362\377\1r\363\377\1s\364\377\1t\366"
  "\377\1t\367\377\202\272\370\377\371\371\371\377\372\372\372\377\372\372\372"
  "\377\373\373\373\377\373\373\373\377\362\367\374\377\31\203\374\377\1w\374"
  "\377\1w\374\377\1v\373\377\1w\373\377\1\243\372\377\1v\372\377\1u\371\377"
  "\1u\370\377\1t\367\377\1t\366\377\1s\364\377\\\243\363\377\362\362\362\377"
  "\360\360\360\377\357\357\357\377\355\355\355\377\354\354\354\377\352\352"
  "\352\377\350\350\350\377\347\347\347\314\377\377\377\4\0\0\0\0\0\0\0\0\0"
  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0q\352$\1m\350\361\1n\351\377\1n\352"
  "\377\1o\354\377\1p\355\377\1p\357\377\1q\360\377\1q\361\377\1r\363\377\1"
  "s\364\377\1s\365\377\307\335\366\377\367\367\367\377\370\370\370\377\370"
  "\370\370\377\371\371\371\377\371\371\371\377\372\372\372\377V\243\372\377"
  "\1v\372\377\1v\372\377\1u\371\377\1u\371\377\1\237\370\377\1x\370\377\1t"
  "\367\377\1t\366\377\1s\365\377\1s\364\377\1r\363\377N\232\361\377\360\360"
  "\360\377\357\357\357\377\355\355\355\377\354\354\354\377\352\352\352\377"
  "\351\351\351\377\350\350\350\363\356\356\356,\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0o\347U\1m\347\374\1n\351\377"
  "\1n\352\377\1o\354\377\1p\355\377\1p\357\377\1q\360\377\1q\361\377\1r\362"
  "\377\1r\363\377\271\324\364\377\365\365\365\377\366\366\366\377\366\366\366"
  "\377\367\367\367\377\367\367\367\377\370\370\370\377L\235\370\377\1u\370"
  "\377\1u\370\377\1t\367\377\1t\367\377\1\231\366\377\1z\366\377\1s\365\377"
  "\1s\364\377\1r\363\377\1r\362\377\1q\361\377W\236\360\377\357\357\357\377"
  "\355\355\355\377\354\354\354\377\352\352\352\377\351\351\351\377\347\347"
  "\347\376\352\352\352W\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0l\347j\1m\347\376\1n\351\377\1n\352"
  "\377\1o\354\377\1p\355\377\1p\356\377\1p\357\377\1q\360\377\1q\361\377Y\240"
  "\362\377\363\363\363\377\364\364\364\377\364\364\364\377\365\365\365\377"
  "\365\365\365\377\334\350\365\377\15z\366\377\1t\366\377\1s\365\377\1s\365"
  "\377\1s\365\377\1\230\364\377\1z\364\377\1r\363\377\1r\362\377\1q\361\377"
  "\1q\360\377\1p\357\377|\261\356\377\355\355\355\377\354\354\354\377\352\352"
  "\352\377\351\351\351\377\347\347\347\375\346\346\346g\0\0\0\0\0\0\0\0\0\0"
  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0\0\0\0\0\0\0o\353s\1m\347\375\1n\351\377\1n\352\377\1o\353\377\1o\354\377"
  "\1p\355\377\1p\357\377\1p\357\377\1q\360\377p\254\361\377\356\360\362\377"
  "\362\362\362\377\363\363\363\377\326\344\363\3775\216\363\377\1s\364\377"
  "\1s\364\377\1r\363\377\1r\363\377\1r\363\377\1\222\362\377\1x\362\377\1q"
  "\361\377\1q\360\377\1p\357\377\1p\357\377\1p\355\377\303\327\354\377\353"
  "\353\353\377\352\352\352\377\351\351\351\377\350\350\350\375\353\353\353"
  "s\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0m\347T\1m\347\364\1m\350"
  "\377\1n\351\377\1o\353\377\1o\354\377\1p\355\377\1p\356\377\1p\356\377\1"
  "p\357\377\24{\360\377N\231\360\377B\224\361\377\10u\361\377\1q\361\377\1"
  "q\361\377\1q\361\377\1q\361\377\1q\361\377\1q\361\377\1\221\360\377\1x\360"
  "\377\1p\357\377\1p\356\377\1p\356\377\1p\355\377D\222\354\377\353\353\353"
  "\377\351\351\351\377\350\350\350\377\347\347\347\366\352\352\352W\0\0\0\0"
  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0p\3520\1n\350\317"
  "\1m\350\377\1n\351\377\1n\352\377\1o\353\377\1o\354\377\1o\354\377\1p\355"
  "\377\1p\356\377\1p\356\377\1p\357\377\1p\357\377\1p\357\377\1p\357\377\1"
  "p\357\377\1p\357\377\1p\357\377\1p\357\377\1\227\356\377\1y\356\377\1p\355"
  "\377\1o\354\377\1o\354\377\21w\353\377\314\332\352\377\351\351\351\377\350"
  "\350\350\377\350\350\350\321\356\356\356,\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0U\377\3\0n\351t\1m\347"
  "\362\1m\350\377\1n\351\377\1n\352\377\1o\353\377\1o\353\377\1o\354\377\1"
  "o\354\377\1p\355\377\1p\355\377\1p\355\377\1p\355\377\1p\355\377\1p\355\377"
  "\1p\355\377\1p\355\377\1q\354\377\1o\354\377\1o\353\377\1o\353\377\14t\352"
  "\377\260\313\351\377\350\350\350\377\350\350\350\360\354\354\354w\377\377"
  "\377\5\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0o\346\36\2m\350\235\1m\347\367\1"
  "m\350\377\1n\351\377\1n\351\377\1n\352\377\1n\352\377\1o\353\377\1o\353\377"
  "\1o\353\377\1o\353\377\1o\353\377\1o\353\377\1o\353\377\1o\353\377\1n\352"
  "\377\1n\352\377\1n\351\377-\204\351\377\277\322\350\377\347\347\347\365\350"
  "\350\350\231\350\350\350\26\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0\0\0\0\0\0\0\0\0\0h\350\26\0n\351t\1m\347\314\1m\347\374\1m\350\377\1m"
  "\350\377\1n\351\377\1n\351\377\1n\351\377\1n\351\377\1n\351\377\1n\351\377"
  "\1n\351\377\1n\351\377\1m\350\377'\201\350\377\206\263\347\375\341\344\347"
  "\303\347\347\347j\343\343\343\22\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0f\335\17"
  "\0j\345M\2m\345\210\1m\347\276\1m\347\343\12r\350\371\20u\351\376\24x\351"
  "\375)\204\351\374D\221\351\365i\245\352\335\235\302\351\242\344\344\347U"
  "\351\351\351\27\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0\0\0",
};

struct sdlappicon sdlappicon = {
	48,48,
	sdlappicon_pixels
};
