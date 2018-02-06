#pragma once
template <class T>
class NullableWrapper {
	public:
		
		NullableWrapper();
		~NullableWrapper();

		T getInternal() const;
		bool isNull() const;
		void reset();
		void set(const T&);


	private:
		bool is_null;
		T internal;
};
