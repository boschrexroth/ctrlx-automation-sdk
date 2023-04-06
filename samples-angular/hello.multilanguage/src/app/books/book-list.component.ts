import { AfterViewInit, Component } from '@angular/core';
import { TranslateService } from '@ngx-translate/core';
import { IBook } from './book';

@Component({
    selector: 'app-books',
    templateUrl: './book-list.component.html',
    styleUrls: ['./book-list.component.scss'],
})
export class ProductListComponent implements AfterViewInit {
    language = 'en';
    myColumns = ['id', 'author', 'title', 'category', 'price'];
    isEnglish = false;
    isGerman = false;

    constructor(private translate: TranslateService) {
        translate.use(this.language);
    }

    ngAfterViewInit() {
        this.enLanguage();
    }

    deLanguage() {
        this.language = 'de';
        this.isGerman = true;
        this.isEnglish = false;
        this.translate.use('de');
    }

    enLanguage() {
        this.language = 'en';
        this.isEnglish = true;
        this.isGerman = false;
        this.translate.use('en');
    }

    books: IBook[] = [
        {
            id: 1,
            author: 'Agatha Christie',
            title: 'doc.book.Title-1',
            category: 'doc.book.Category-1',
            price: 16.45,
        },
        {
            id: 2,
            author: 'Rosamunde Pilcher',
            title: 'doc.book.Title-2',
            category: 'doc.book.Category-2',
            price: 12.51,
        },
        {
            id: 3,
            author: 'J. R. R. Tolkien',
            title: 'doc.book.Title-3',
            category: 'doc.book.Category-3',
            price: 72.98,
        },
        {
            id: 4,
            author: 'Brian Johnson',
            title: 'doc.book.Title-4',
            category: 'doc.book.Category-4',
            price: 29.95,
        },
        {
            id: 5,
            author: 'Matthew Ray Russell',
            title: 'doc.book.Title-5',
            category: 'doc.book.Category-5',
            price: 25.71,
        },
        {
            id: 6,
            author: 'Rob Brunia',
            title: 'doc.book.Title-6',
            category: 'doc.book.Category-6',
            price: 11.39,
        },
    ];
}
