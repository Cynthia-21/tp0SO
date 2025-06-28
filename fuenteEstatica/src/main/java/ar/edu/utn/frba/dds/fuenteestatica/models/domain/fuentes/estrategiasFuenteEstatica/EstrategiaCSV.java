package ar.edu.utn.frba.dds.fuenteestatica.models.domain.fuentes.estrategiasFuenteEstatica;

import ar.edu.utn.frba.dds.fuenteestatica.models.domain.dtos.input.HechoFormatoOriginal;
import com.opencsv.bean.CsvToBean;
import com.opencsv.bean.CsvToBeanBuilder;
import ar.edu.utn.frba.dds.fuenteestatica.models.domain.Categoria;
import ar.edu.utn.frba.dds.fuenteestatica.models.domain.Hecho;
import ar.edu.utn.frba.dds.fuenteestatica.models.domain.exceptions.ErrorImportacionCSVException;
import ar.edu.utn.frba.dds.fuenteestatica.models.domain.origen.Origen;
import ar.edu.utn.frba.dds.fuenteestatica.models.domain.ubicacion.Coordenadas;
import ar.edu.utn.frba.dds.fuenteestatica.models.domain.ubicacion.Ubicacion;
import lombok.AllArgsConstructor;
import lombok.Getter;
import lombok.Setter;

import java.io.FileReader;
import java.time.LocalDate;
import java.time.format.DateTimeFormatter;
import java.util.ArrayList;
import java.util.List;

@Getter
@Setter
@AllArgsConstructor
public class EstrategiaCSV implements EstrategiaImportacionEstatica{

  private Archivo archivo;
  private Integer limiteDeRegistrosImportables;
  @Override
  public List<Hecho> leerArchivo(String rutaFuente) {
    List<Hecho> hechosImportados = new ArrayList<>();
    String rutaCompleta = rutaFuente + "/" + archivo.getNombre();
    try (FileReader reader = new FileReader(rutaCompleta)) {
      CsvToBean<HechoFormatoOriginal> csvToBean = new CsvToBeanBuilder<HechoFormatoOriginal>(reader)
          .withType(HechoFormatoOriginal.class)
          .withIgnoreLeadingWhiteSpace(true)
          .withThrowExceptions(false)
          .build();
      int contador = 0;

      DateTimeFormatter formatter = DateTimeFormatter.ofPattern("dd/MM/yyyy");

      for (HechoFormatoOriginal hechoLeido : csvToBean) {
        Hecho hechoNuevo = ArmarHecho(hechoLeido, formatter);
        hechosImportados.add(hechoNuevo);
        contador++;
        if (limiteDeRegistrosImportables > 0 && contador >= limiteDeRegistrosImportables) break;
      }
    } catch (Exception e) {
      throw new ErrorImportacionCSVException(e);
    }
    return hechosImportados;
  }

  private Hecho ArmarHecho(HechoFormatoOriginal hechoLeido, DateTimeFormatter formatter) {
    Coordenadas coord = new Coordenadas(Double.parseDouble(hechoLeido.getLatitud()), Double.parseDouble(hechoLeido.getLongitud()));
    Ubicacion nuevaUbi = new Ubicacion(coord);
    Categoria nuevaCategoria = new Categoria(hechoLeido.getCategoria());
    Hecho hechoNuevo = new Hecho(hechoLeido.getTitulo());
    hechoNuevo.setUbicacion(nuevaUbi);
    hechoNuevo.setCategorias(List.of(nuevaCategoria));
    hechoNuevo.setFechaAcontecimiento(LocalDate.parse(hechoLeido.getFechaDelHecho(), formatter));
    hechoNuevo.setOrigen(Origen.DATASET);
    hechoNuevo.setArchivoOrigen(this.getArchivo());
    return hechoNuevo;
  }
}

