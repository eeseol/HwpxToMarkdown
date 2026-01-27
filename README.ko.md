# hwpx-owpml-model

## 이 프로젝트는 무엇인가?

`hwpx-owpml-model`은 HWPX(OWPML) 문서를 구조적으로 파싱하고 모델링하기 위한 경량 라이브러리입니다.

이 프로젝트는 다음을 제공합니다:

* OWPML(OOXML 기반) 문서 모델
* 문서 엘리먼트(섹션, 문단, 런, 표 등)의 트리 구조 표현
* 텍스트 및 레이아웃 정보를 추출할 수 있습니다.

본 라이브러리는 다음과 같은 용도로 사용되도록 설계되었습니다:

* HWPX → html 변환기
* RAG(검색 기반 생성) 문서 전처리 파이프라인
* 검색 및 인덱싱 시스템
* AI 기반 문서 이해 및 요약

---

## 왜 이 프로젝트가 필요한가?

HWPX(OWPML)는 XML 기반의 강력한 포맷이지만 구조가 매우 복잡하여,
RAG, 검색, 요약 등의 파이프라인에서 바로 사용하기 어렵습니다.

이 프로젝트는:

* OWPML을 구조화된 객체 모델로 변환하고
* 섹션, 문단, 표 등의 요소를 안정적으로 순회할 수 있게 하여
* 룰 기반 및 AI 기반 후처리가 가능하도록 합니다.

---

## 주요 기능

* 문서 내 텍스트 추출
* outline(개요1~10) -> h1,, 태그 표현
* 표 레이아웃 및 텍스트 표현
* 리스트 표현

---

## 1. 시작하기 (Release)
코드 클론해서 쓰는게 아니라 release? 그거 쓰는 방법 설명


## 2. 시작하기 (Windows 빌드)

본 프로젝트는 한컴 공개 레포 SDK를 사용합니다.
한컴 SDK lib 파일이 필요합니다.

https://github.com/hancom-io/hwpx-owpml-model 여기 가서 깃 클론을 하든 포크를 하든 하세요.

### 빌드 환경

* Microsoft Windows 11
* Microsoft Visual Studio 2022
* 플랫폼: x86

### 빌드 방법

1. Visual Studio에서 솔루션 열기
2. 구성 선택: `Release`
3. 플랫폼 선택: `x86`
4. 빌드

빌드가 완료되면 `Build/Bin` 폴더에 다음 파일이 생성됩니다:

* `Owpml.lib`
* `OWPMLApi.lib`
* `OWPMLUtil.lib`

이 라이브러리들을 복사하여, 본 레포의 include 폴더에 붙여넣어 주세요.

---

## 샘플 실행

### Visual Studio에서 실행

이거 test 폴더 만들어서 내가만든 hwpx 파일 하나 넣어두려고..

```
입력파일.hwpx 출력파일.txt
```

### 커맨드라인에서 실행

```bash
OWPMLTest.exe InputFile.hwpx OutputFile.txt
```

첫 번째 섹션의 텍스트가 출력 파일에 저장됩니다.

---

## 문서 파이프라인에서의 활용

이 라이브러리는 다음과 같은 시스템의 **저수준 파서**로 사용되도록 설계되었습니다:

* HWPX → Markdown 변환기
* RAG(문서 청킹, 임베딩, 검색)
* 검색 및 인덱싱 시스템
* AI 기반 문서 요약 및 분석

문서의 구조적 표현을 제공함으로써
룰 기반 처리와 AI 기반 처리를 모두 가능하게 합니다.

---

## 기여 가이드

### 코드 스타일

* 들여쓰기는 탭이 아닌 스페이스
* UTF-8 인코딩

### 커밋 규칙

* 제목과 본문 사이 한 줄 띄우기
* 제목 50자 이내
* 형식: `<영역>: <요약>`
  예: `engine: Improve OWPML table traversal`
* 한글은 명사형, 영어는 명령형
* 본문은 72자 줄바꿈
* 무엇보다 **왜, 어떻게**를 중심으로 작성

---

## 브랜치 및 작업 흐름

### 클론

```bash
git clone https://github.com/hancom-io/hwpx-owpml-model.git
```

### 브랜치 생성

```bash
git checkout -b <브랜치이름>
git push origin <브랜치이름>
```

### 푸시 및 PR

```bash
git status
git add .
git commit -m "#<이슈번호> <메시지>"
git push origin <브랜치이름>
```

Pull Request를 통해 main 브랜치로 병합합니다.

---

## 라이선스

자세한 내용은 [LICENSE.txt](LICENSE.txt)를 참고하세요.

---

## 참고 프로젝트

본 프로젝트는 한컴에서 공개한 공식 OWPML 참조 구현을 기반으로 합니다:

https://github.com/hancom-io/hwpx-owpml-model

OWPML 문서 구조 및 모델 정의를 토대로,
문서 변환 및 AI/RAG 파이프라인에 활용할 수 있도록 확장·재구성하였습니다.

---

## 문의

질문 및 토론은 GitHub Discussions를 이용해주세요.

---

