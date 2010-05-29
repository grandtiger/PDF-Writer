#include "SimpleContentPageTest.h"
#include "PDFWriter.h"
#include "PDFPage.h"
#include "PDFRectangle.h"
#include "PageContentContext.h"
#include "TestsRunner.h"

#include <iostream>

using namespace std;

SimpleContentPageTest::SimpleContentPageTest(void)
{
}

SimpleContentPageTest::~SimpleContentPageTest(void)
{
}

/*
	Creates a single page PDF with a blue rectangle
*/

EStatusCode SimpleContentPageTest::Run()
{
	PDFWriter pdfWriter;
	EStatusCode status; 

	do
	{

		status = pdfWriter.InitializePDFWriter(L"C:\\PDFLibTests\\SimpleContent.PDF");
		if(status != eSuccess)
		{
			wcout<<"failed to initialize PDF Writer\n";
			break;
		}

		status = pdfWriter.StartPDF(ePDFVersion13);
		if(status != eSuccess)
		{
			wcout<<"failed to start PDF\n";
			break;
		}	

		PDFPage* page = new PDFPage();
		page->SetMediaBox(PDFRectangle(0,0,595,842));

		PageContentContext* contentContext = pdfWriter.StartPageContentContext(page);
		if(NULL == contentContext)
		{
			status = eFailure;
			wcout<<"failed to create content context for page\n";
		}

		// draw a 100X100 points cyan square
		contentContext->q();
		contentContext->k(100,0,0,0);
		contentContext->re(100,500,100,100);
		contentContext->f();
		contentContext->Q();

		// force stream change
		status = pdfWriter.PausePageContentContext(contentContext);
		if(status != eSuccess)
		{
			wcout<<"failed to pause page content context\n";
			break;
		}

		// draw a 200X100 points red rectangle
		contentContext->q();
		contentContext->k(0,100,100,0);
		contentContext->re(200,600,200,100);
		contentContext->f();
		contentContext->Q();

		// draw a gray line
		contentContext->q();
		contentContext->G(0.5);
		contentContext->w(3);
		contentContext->m(200,600);
		contentContext->l(400,400);
		contentContext->S();
		contentContext->Q();
		
		status = pdfWriter.EndPageContentContext(contentContext);
		if(status != eSuccess)
		{
			wcout<<"failed to end page content context\n";
			break;
		}

		status = pdfWriter.WritePageAndRelease(page);
		if(status != eSuccess)
		{
			wcout<<"failed to write page\n";
			break;
		}

		status = pdfWriter.EndPDF();
		if(status != eSuccess)
		{
			wcout<<"failed in end PDF\n";
			break;
		}
	}while(false);
	return status;
}

ADD_CETEGORIZED_TEST(SimpleContentPageTest,"PDF")
