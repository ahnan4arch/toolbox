/*** 自動登録ファクトリ
 * 親クラスにstaticなFACTORY<親クラス>* FACTORY<親クラス>::start(0)を定義
 * 子クラスにstaticなFACTORY<親クラス> 子クラス::factory(New)を定義
 * 子クラスにstaticな親クラス* New()を定義
 * 親クラスからFACTORY<親クラス>::Newを呼び出すと子クラスのNewを順に呼んで
 * 値が入ってたらそれを返してくれる
 *
 * NOTE:一般的にFACTORY<親クラス>::Newを呼ぶのはmainに入った後にすること
 * 登録がグローバルコンストラクタなのでグローバルコンストラクタでNewするには
 * 呼び出し順を調整する必要がある
 */


#pragma once


template<typename T> class FACTORY{
	FACTORY();
	FACTORY(const FACTORY&);
	void operator=(const FACTORY&);
public:
	FACTORY(T* (*f)()) :
		next(start),
		factory(f){
			start = this;
		};
	static T* New(){
		for(FACTORY* f(start); f; f = (*f).next){
			T* const t(((*f).factory)());
			if(t){
				return t;
			}
		}
		return 0;
	};
private:
	static FACTORY* start;
	FACTORY* const next;
	T* (*const factory)();;
};

