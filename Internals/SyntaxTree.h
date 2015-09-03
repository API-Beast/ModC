//  Copyright (C) 2014 Manuel Riecke <spell1337@gmail.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#pragma once

#include <vector>
#include <string>

namespace ModC
{
namespace AST
{
	
	struct Token
	{
		enum EType
		{
			TSymbol, Reserved, ParenOpen, ParenClose, Operator, Keyword,
			TValue,  Identifier, Number, String,
			Unknown
		};
		EType Type = Token::Unknown;
		std::string Content;
		bool operator==(Token::EType t) const       { return Type == t;    };
		bool operator==(const std::string& str)const{ return Content == str; };
		bool operator==(const Token& t) const       { return Type == t.Type && Content == t.Content; };
		bool isValue () const { return Type >= TValue  && Type < Unknown; };
		bool isSymbol() const { return Type >= TSymbol && Type < TValue;  };
	};

	struct Expression;
	using ExpressionList = std::vector<Expression>;
	
	struct Expression
	{
		Expression();
		
		enum EType
		{
			Definition,
			Collection,
			Operator,
			FlowStatement,
			Call,
			SingleValue,
			Consumed
		};
		EType Type = Expression::SingleValue;
		Token Tok;
		ExpressionList Right;
		ExpressionList Left;
		
		bool isSymbol()  const { return Type == SingleValue && Tok.isSymbol(); }
		bool isValue()   const { return Type == SingleValue && Tok.isValue();  }
		bool isKeyword() const { return Type == SingleValue && Tok.Type == Token::Keyword; };
		bool isBinOp()   const { return Type == Operator    && Right.size() == 1 && Left.size() == 1; };
		bool isPreOp()   const { return Type == Operator    && Right.size() == 1 && Left.size() == 0; };
		bool isPostOp()  const { return Type == Operator    && Right.size() == 0 && Left.size() == 1; };
		bool resolved()      const;
		bool fullyResolved() const;
	};
	
	struct Iterator
	{
		int Position = 0;
		int Start = 0;
		int End;
		ExpressionList* Target;
		
		Iterator(){};
		Iterator(ExpressionList* target);
		bool atEnd()         { return Position + 1 > End || Position >= (*Target).size(); }
		int  remainingItems(){ return End - Position;                                     };
		
		Expression& value(int pos = 0){ return (*Target)[Position+pos]; };
		
		void advance(){ Position++; };
		
		
		bool isKeywordL(int arguments = 1);
		bool isKeywordR(int arguments = 1);
		
		bool canConsumeL(int arguments = 1);
		bool canConsumeR(int arguments = 1);
		
		void consumeL(int arguments = 1);
		void consumeR(int arguments = 1);
		
		void eraseR(int arguments = 1);
		void merge(const Iterator& end);
	};
	
}
}