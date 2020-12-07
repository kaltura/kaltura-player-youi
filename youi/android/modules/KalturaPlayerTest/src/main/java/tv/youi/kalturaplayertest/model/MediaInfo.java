package tv.youi.kalturaplayertest.model;

import com.kaltura.playkit.PKDrmParams;
import com.kaltura.playkit.PKMediaEntry;
import com.kaltura.playkit.PKMediaFormat;

public class MediaInfo {

    private String mediaId;
    private String uri;
    private PKMediaEntry.MediaEntryType mediaType;
    private PKMediaFormat mediaFormat;
    private PKDrmParams drmData;

    public String getMediaId() {
        return mediaId;
    }

    public void setMediaId(String mediaId) {
        this.mediaId = mediaId;
    }

    public String getUri() {
        return uri;
    }

    public void setUri(String uri) {
        this.uri = uri;
    }

    public PKMediaEntry.MediaEntryType getMediaType() {
        return mediaType;
    }

    public void setMediaType(PKMediaEntry.MediaEntryType mediaType) {
        this.mediaType = mediaType;
    }

    public PKMediaFormat getMediaFormat() {
        return mediaFormat;
    }

    public void setMediaFormat(PKMediaFormat mediaFormat) {
        this.mediaFormat = mediaFormat;
    }

    public PKDrmParams getDrmData() {
        return drmData;
    }

    public void setDrmData(PKDrmParams drmData) {
        this.drmData = drmData;
    }
}