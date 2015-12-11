FORMS           = GoldBug-UI\\adaptiveechoprompt.ui \
		  GoldBug-UI\\buzzpage.ui \
		  GoldBug-UI\\chatwindow.ui \
		  GoldBug-UI\\controlcenter.ui \
		  GoldBug-UI\\echo-key-share.ui \
		  GoldBug-UI\\encryptfile.ui \
		  GoldBug-UI\\forwardsecrecyalgorithmsselection.ui \
		  GoldBug-UI\\ipinformation.ui \
		  GoldBug-UI\\keyboard.ui \
		  GoldBug-UI\\listener-socket-options.ui \
                  GoldBug-UI\\logviewer.ui \
		  GoldBug-UI\\options.ui \
		  GoldBug-UI\\pageviewer.ui \
                  GoldBug-UI\\passwordprompt.ui \
		  GoldBug-UI\\poptastic-retrophone-settings.ui \
		  GoldBug-UI\\postgresqlconnect.ui \
		  GoldBug-UI\\rosetta.ui \
                  GoldBug-UI\\starbeamanalyzer.ui \
		  GoldBug-UI\\statisticswindow.ui \
		  GoldBug-UI\\statusbar.ui \
		  GoldBug-UI\\unlock.ui

UI_HEADERS_DIR  = GUI

HEADERS		= Common\\spot-on-external-address.h \
		  GUI\\spot-on.h \
		  GUI\\spot-on-buzzpage.h \
		  GUI\\spot-on-chatwindow.h \
		  GUI\\spot-on-echo-key-share.h \
		  GUI\\spot-on-encryptfile.h \
		  GUI\\spot-on-logviewer.h \
		  GUI\\spot-on-rosetta.h \
                  GUI\\spot-on-starbeamanalyzer.h \
                  GUI\\spot-on-tabwidget.h \
		  GUI\\spot-on-textbrowser.h \
		  GUI\\spot-on-textedit.h

SOURCES		= Common\\spot-on-crypt.cc \
		  Common\\spot-on-crypt-mceliece.cc \
		  Common\\spot-on-crypt-ntru.cc \
		  Common\\spot-on-external-address.cc \
		  Common\\spot-on-misc.cc \
                  Common\\spot-on-send.cc \
		  GUI\\spot-on-a.cc \
		  GUI\\spot-on-b.cc \
		  GUI\\spot-on-buzzpage.cc \
		  GUI\\spot-on-c.cc \
		  GUI\\spot-on-chatwindow.cc \
		  GUI\\spot-on-d.cc \
		  GUI\\spot-on-e.cc \
		  GUI\\spot-on-echo-key-share.cc \
		  GUI\\spot-on-encryptfile.cc \
		  GUI\\spot-on-f.cc \
		  GUI\\spot-on-logviewer.cc \
		  GUI\\spot-on-reencode.cc \
		  GUI\\spot-on-rosetta.cc \
		  GUI\\spot-on-smp.cc \
                  GUI\\spot-on-starbeamanalyzer.cc \
		  GUI\\spot-on-tabwidget.cc \
		  GUI\\spot-on-textbrowser.cc \
		  GUI\\spot-on-textedit.cc \
		  GUI\\spot-on-urls.cc \
		  GUI\\spot-on-urls-search.cc

TRANSLATIONS    = Translations\\spot-on_af.ts \
                  Translations\\spot-on_al.ts \
                  Translations\\spot-on_am.ts \
                  Translations\\spot-on_ar.ts \
                  Translations\\spot-on_as.ts \
                  Translations\\spot-on_az.ts \
                  Translations\\spot-on_be.ts \
                  Translations\\spot-on_bd.ts \
                  Translations\\spot-on_bg.ts \
                  Translations\\spot-on_ca.ts \
                  Translations\\spot-on_cr.ts \
                  Translations\\spot-on_cz.ts \
                  Translations\\spot-on_de.ts \
                  Translations\\spot-on_dk.ts \
                  Translations\\spot-on_ee.ts \
                  Translations\\spot-on_es.ts \
                  Translations\\spot-on_eo.ts \
                  Translations\\spot-on_et.ts \
                  Translations\\spot-on_eu.ts \
                  Translations\\spot-on_fi.ts \
                  Translations\\spot-on_fr.ts \
                  Translations\\spot-on_gl.ts \
                  Translations\\spot-on_gr.ts \
                  Translations\\spot-on_hb.ts \
                  Translations\\spot-on_hi.ts \
                  Translations\\spot-on_hr.ts \
                  Translations\\spot-on_hu.ts \
                  Translations\\spot-on_it.ts \
                  Translations\\spot-on_il.ts \
                  Translations\\spot-on_ie.ts \
                  Translations\\spot-on_id.ts \
                  Translations\\spot-on_ja.ts \
                  Translations\\spot-on_kk.ts \
                  Translations\\spot-on_kn.ts \
                  Translations\\spot-on_ko.ts \
                  Translations\\spot-on_ky.ts \
                  Translations\\spot-on_ku.ts \
                  Translations\\spot-on_lt.ts \
                  Translations\\spot-on_lk.ts \
                  Translations\\spot-on_lv.ts \
                  Translations\\spot-on_ml.ts \
                  Translations\\spot-on_mk.ts \
                  Translations\\spot-on_mn.ts \
                  Translations\\spot-on_ms.ts \
                  Translations\\spot-on_my.ts \
                  Translations\\spot-on_mr.ts \
                  Translations\\spot-on_mt.ts \
                  Translations\\spot-on_nl.ts \
                  Translations\\spot-on_no.ts \
                  Translations\\spot-on_np.ts \
                  Translations\\spot-on_pl.ts \
                  Translations\\spot-on_pa.ts \
                  Translations\\spot-on_pt.ts \
                  Translations\\spot-on_ps.ts \
                  Translations\\spot-on_ro.ts \
                  Translations\\spot-on_ru.ts \
                  Translations\\spot-on_rw.ts \
                  Translations\\spot-on_sv.ts \
                  Translations\\spot-on_sk.ts \
                  Translations\\spot-on_sl.ts \
                  Translations\\spot-on_sr.ts \
                  Translations\\spot-on_sq.ts \
                  Translations\\spot-on_sw.ts \
                  Translations\\spot-on_th.ts \
                  Translations\\spot-on_tr.ts \
                  Translations\\spot-on_vn.ts \
                  Translations\\spot-on_zh.ts \
                  Translations\\spot-on_zh_TW.ts \
                  Translations\\spot-on_zh_HK.ts