package ar.edu.utn.frba.dds.fuentedinamica.models.dtos.output;

import ar.edu.utn.frba.dds.fuentedinamica.models.domain.Categoria;
import lombok.Data;

import java.time.LocalDate;
import java.time.LocalDateTime;
import java.util.List;

@Data
public class HechoOutputDTO {
  private Long id;
  private String titulo;
  private String descripcion;
  private List<Categoria> categorias;
  private String contenidoMultimedia;
  private String ubicacion;
  private LocalDate fechaAcontecimiento;
  private LocalDateTime fechaDeCarga;
  private String origen;
  private String contribuyente;
  private List<String> etiquetas;
}
